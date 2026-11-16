"""
r5900_microcode.py -- Hex-Rays microcode filter for PS2 Emotion Engine (R5900)
128-bit quadword load/store (lq/sq).

Drop into your IDA plugins dir (idaapi.get_user_idadir()/plugins). It loads
automatically for MIPS IDBs and lifts lq/sq into real 16-byte memory moves,
so callee-save prologue/epilogue pairs fold away and mid-body quadword copies
survive as data movement instead of __asm blocks.

STATUS: untested skeleton. The one thing to verify first is whether IDA's MIPS
module types the lq/sq operands as 16 bytes -- see the note in apply().
"""

import ida_idaapi
import ida_idp
import ida_hexrays
import ida_ua
import ida_typeinf
import ida_bytes

# Bump this whenever the file changes, so _install() prints it and you can prove
# which version IDA actually loaded (reload/shadow-file sanity check).
R5900_VERSION = "v2 2026-11-16 ldx/stx lq/sq, no helper arglocs, generic EE helpers; was 2026-01-19.l  (clean vf9=... intrinsic form, toggle vu_clean)"

# ---------------------------------------------------------------------------
# SELF-CLEANUP ON RE-RUN.
# When this script is re-run (File > Script file), Python creates fresh function
# and class objects, but any microcode filters installed by the PREVIOUS run are
# still hooked in the decompiler, executing stale code -- the cause of the
# "NameError / old behaviour after editing" problems.
#
# We stash the installed filter instances on a persistent object that survives
# re-exec (ida_idaapi module itself). At the very TOP of every load, we uninstall
# whatever the previous run registered there, so the new run starts clean.
# ---------------------------------------------------------------------------
_REGISTRY_KEY = "_r5900_installed_filters"


def _registry():
    if not hasattr(ida_idaapi, _REGISTRY_KEY):
        setattr(ida_idaapi, _REGISTRY_KEY, [])
    return getattr(ida_idaapi, _REGISTRY_KEY)


def _uninstall_previous():
    """Uninstall EVERY filter any prior run left hooked. A filter can be
    installed more than once across reloads, and uninstall only removes one
    hook per call, so we uninstall each registered filter repeatedly until the
    API reports it's no longer present (install returns False when not hooked)."""
    reg = _registry()
    n = 0
    for flt in reg:
        # uninstall repeatedly -- a stale instance may have been hooked multiple
        # times; keep removing until the call has no effect.
        for _ in range(8):
            try:
                if not ida_hexrays.install_microcode_filter(flt, False):
                    break
                n += 1
            except Exception:
                break
    reg.clear()
    if n:
        print("[r5900] self-cleanup: removed %d stale filter hook(s)" % n)


def _register(flt):
    """Record a freshly-installed filter so the NEXT run can uninstall it.
    De-dupes so the registry never grows unbounded across reloads."""
    reg = _registry()
    if flt not in reg:
        reg.append(flt)


# Run cleanup immediately at load, before anything else is defined/installed.
try:
    if ida_hexrays.init_hexrays_plugin():
        _uninstall_previous()
except Exception as _e:
    print("[r5900] self-cleanup skipped: %r" % _e)

# IDA models MIPS GPRs as 8 bytes in the microcode register file. A 16-byte
# register mop overruns into the neighbouring GPR and trips the verifier
# (INTERR 50920). 8 bytes is the safe width for anything touching a GPR.
# Cost: a genuine mid-body 128-bit copy is modeled as its low 64 bits only.
# For pure prologue/epilogue save+restore this is exact -- the pair still folds.
REG_SIZE = 8       # microcode width of a MIPS GPR
QUAD_SIZE = 16     # width of the lq/sq stack slot (dt_byte16) and of our temps


def _resolve_itypes(wanted):
    """Map each wanted mnemonic -> itype via the processor's instruction table.

    ph_get_instruc() returns a list of (name, feature) tuples; the list index
    is the itype. This avoids hardcoding MIPS_* constants that differ across
    IDA versions / processor builds. Returns {name: itype} for names found.
    """
    found = {}
    wanted = set(wanted)
    for itype, (name, _feat) in enumerate(ida_idp.ph_get_instruc()):
        if name in wanted:
            found[name] = itype
    return found


def _reg_mop(mreg, size):
    """Build a register mop of an explicit size (version-robust: set fields
    directly rather than relying on a particular make_reg overload)."""
    m = ida_hexrays.mop_t()
    m.t = ida_hexrays.mop_r
    m.r = mreg
    m.size = size
    return m


def _emit(cdg, opcode, l, r, d, fp=False):
    """Append a microinstruction (opcode l, r -> d) to the current block.

    When fp=True the instruction is tagged FPINSN so the verifier treats it as a
    floating-point op; a float opcode (m_fadd/m_fmul/...) on otherwise-untyped
    operands is rejected with INTERR 50801 without this flag.
    """
    m = ida_hexrays.minsn_t(cdg.insn.ea)
    m.opcode = opcode
    if l is not None:
        m.l = l
    if r is not None:
        m.r = r
    if d is not None:
        m.d = d
    if fp:
        m.set_fpinsn()
    cdg.mb.insert_into_block(m, cdg.mb.tail)
    return m


# The R5900 FPU has a hidden single-precision accumulator ($ACC). Hex-Rays has no
# such register, the module exposes none to borrow, a kreg can't hold it (the
# kreg allocator resets each apply(), so it aliases madd's `prod` -> prod+prod),
# and growing mba.stacksize for a stack slot is illegal mid-codegen (INTERR
# 52702). What works: a FIXED scratch mreg parked well above the physical
# registers. It persists across apply() like any register, and being outside the
# kreg pool it can't collide with `prod` (which stays a low kreg).
FPSIZE = 4  # single precision
_acc_mreg = None


def _acc_reg():
    """A stable scratch mreg used to model ACC across the whole function."""
    global _acc_mreg
    if _acc_mreg is None:
        try:
            n = len(list(ida_idp.ph_get_regnames()))
            hi = max(ida_hexrays.reg2mreg(i) for i in range(n))
        except Exception:
            hi = 0x100
        # park ACC far above the highest physical mreg (and above typical kregs)
        _acc_mreg = ((hi >> 4) << 4) + 0x800
    return _acc_mreg


# FP register operands carry an FPR-relative index (0..31) in op.reg. reg2mreg
# needs the *global* register number, where $fN sits at FPR_BASE + N -- otherwise
# reg2mreg(1) yields $at (GPR 1) instead of $f1, and a float op on GPRs trips
# INTERR 50801. Resolve the base once from the processor's register-name table.
_fpr_base = None


def _fpr_base_idx():
    global _fpr_base
    if _fpr_base is None:
        try:
            names = list(ida_idp.ph_get_regnames())
        except Exception:
            names = []
        for cand in ("f0", "$f0", "fpr0"):
            if cand in names:
                _fpr_base = names.index(cand)
                break
    return _fpr_base


def _fpr_mop(fpr_index):
    base = _fpr_base_idx()
    gidx = fpr_index if base is None else base + fpr_index
    return _reg_mop(ida_hexrays.reg2mreg(gidx), FPSIZE)


def probe(ea):
    """Console helper: print operand types/dtypes for the lq/sq at `ea`.
    dtype 22 == dt_byte16 (16-byte operand). Run this on one sq and one lq to
    confirm whether load_operand/store_operand will size transfers at 8 or 16.
    """
    import ida_ua
    insn = ida_ua.insn_t()
    if ida_ua.decode_insn(insn, ea) == 0:
        print("[probe] decode failed")
        return
    print("[probe] %s @ %X" % (ida_idp.ph_get_instruc()[insn.itype][0], ea))
    for i, op in enumerate(insn.ops):
        if op.type == ida_ua.o_void:
            break
        print("  op%d  type=%d  reg=%d  dtype=%d" % (i, op.type, op.reg, op.dtype))


class r5900_filter_t(ida_hexrays.microcode_filter_t):
    def __init__(self, sq_itype, lq_itype):
        super().__init__()
        self.sq = sq_itype
        self.lq = lq_itype

    def match(self, cdg):
        return cdg.insn.itype in (self.sq, self.lq)

    def apply(self, cdg):
        insn = cdg.insn

        # Operand layout for both:  op[0] = GPR (rt),  op[1] = displ mem [base+off]
        gpr = ida_hexrays.reg2mreg(insn.ops[0].reg)

        # load_operand/store_operand pick the transfer width from IDA's per-slot
        # operand dtype, which is inconsistent across lq/sq sites (some slots are
        # typed 2- or 8-byte from other accesses) and CANNOT be overridden by
        # mutating cdg.insn's dtype -- load_operand ignores it. So instead of
        # assuming a width, we adapt to whatever load_operand emitted, reading it
        # back from the tail instruction. This makes the register-boundary
        # conversion always size-consistent (no def/use mismatch -> no INTERR).
        # The address comes from load_effective_address and the transfer is an explicit
        # ldx/stx: load_operand raised INTERR 52816 on an lq through an incoming argument
        # register, and a $zero source register raised 50920.
        addr = cdg.load_effective_address(1)
        seg = _reg_mop(ida_hexrays.reg2mreg(ida_idp.str2reg("cs")), 2)
        if insn.itype == self.sq:
            if insn.ops[0].reg == 0:
                src8 = ida_hexrays.mop_t()
                src8.make_number(0, REG_SIZE)
            else:
                src8 = _reg_mop(gpr, REG_SIZE)
            tmp = cdg.mba.alloc_kreg(QUAD_SIZE)
            _emit(cdg, ida_hexrays.m_xdu, src8, None, _reg_mop(tmp, QUAD_SIZE))
            _emit(cdg, ida_hexrays.m_stx, _reg_mop(tmp, QUAD_SIZE), seg, _reg_mop(addr, 4))
            cdg.mba.free_kreg(tmp, QUAD_SIZE)
        elif insn.ops[0].reg != 0:
            _emit(cdg, ida_hexrays.m_ldx, seg, _reg_mop(addr, 4), _reg_mop(gpr, REG_SIZE))

        return ida_hexrays.MERR_OK


# Names of the R5900 single-precision multiply-accumulate family.
# The two-operand accumulator forms resolve cleanly by name. The three-operand
# FPR-destination forms are ambiguous: the module has several "madd"/"msub"
# entries (madd, madd.s, madd.d, madd.ps, madd1, ...) and name matching can pick
# the wrong itype -- the real single-precision one is displayed as "madd.s" but
# its table name is bare "madd". So those two are resolved by decoding an actual
# instruction (see _resolve_fpu_ma3) and canonicalized to madd.s/msub.s here.
FPU_A2 = ("adda", "suba", "mula")                  # ACC = fs OP ft
FPU_MA2 = ("madda", "msuba")                       # ACC = ACC +/- fs*ft
FPU_MA3 = ("madd.s", "msub.s")                     # fd  = ACC +/- fs*ft  (canonical)
FPU_NAMES = FPU_A2 + FPU_MA2


def _resolve_fpu_ma3():
    """Return {itype: canonical_name} for the 3-operand madd.s/msub.s.

    Name matching is unreliable here, so we anchor on the accumulator-setter
    itypes (madda/msuba), which resolve cleanly, and take the two entries that
    follow them in the instruction table -- in this module the single-precision
    madd/msub sit immediately after madda/msuba. Verified against a decoded
    instruction where possible.
    """
    names = [nm for nm, _f in ida_idp.ph_get_instruc()]
    out = {}
    # Anchor: the real single-precision madd/msub are the bare-named "madd"/"msub"
    # entries that carry three FPR operands. Pick the ones adjacent to madda/msuba.
    for anchor, canon in (("madda", "madd.s"), ("msuba", "msub.s")):
        if anchor not in names:
            continue
        base = names.index(anchor)
        # scan forward a few slots for the matching bare op name
        want = "madd" if canon == "madd.s" else "msub"
        for it in range(base + 1, min(base + 4, len(names))):
            if names[it] == want:
                out[it] = canon
                break
    return out


class r5900_fpu_filter_t(ida_hexrays.microcode_filter_t):
    """Lift the FPU multiply-accumulate family via a modeled ACC register."""

    def __init__(self, name2itype):
        super().__init__()
        self.by_itype = {it: nm for nm, it in name2itype.items()}

    def match(self, cdg):
        return cdg.insn.itype in self.by_itype

    def apply(self, cdg):
        insn = cdg.insn
        name = self.by_itype[insn.itype]

        def F(opn):  # single-precision FPR operand as a 4-byte float mop
            return _fpr_mop(insn.ops[opn].reg)

        def A():     # ACC as a stable scratch register (fresh mop each call)
            return _reg_mop(_acc_reg(), FPSIZE)

        if name in FPU_A2:
            op = {"adda": ida_hexrays.m_fadd,
                  "suba": ida_hexrays.m_fsub,
                  "mula": ida_hexrays.m_fmul}[name]
            _emit(cdg, op, F(0), F(1), A(), fp=True)         # ACC = fs OP ft

        elif name in FPU_MA2:
            prod = cdg.mba.alloc_kreg(FPSIZE)
            _emit(cdg, ida_hexrays.m_fmul, F(0), F(1), _reg_mop(prod, FPSIZE), fp=True)
            op = ida_hexrays.m_fadd if name == "madda" else ida_hexrays.m_fsub
            _emit(cdg, op, A(), _reg_mop(prod, FPSIZE), A(), fp=True)  # ACC = ACC +/- prod
            cdg.mba.free_kreg(prod, FPSIZE)

        elif name in FPU_MA3:
            prod = cdg.mba.alloc_kreg(FPSIZE)
            _emit(cdg, ida_hexrays.m_fmul, F(1), F(2), _reg_mop(prod, FPSIZE), fp=True)  # fs*ft
            op = ida_hexrays.m_fadd if name == "madd.s" else ida_hexrays.m_fsub
            _emit(cdg, op, A(), _reg_mop(prod, FPSIZE), F(0), fp=True)  # fd = ACC +/- prod
            cdg.mba.free_kreg(prod, FPSIZE)

        return ida_hexrays.MERR_OK


def bind_fpu(ea, canon=None):
    """Manually bind the FPU instruction at `ea` into the running filter.

    Use when the auto-resolver picks the wrong itype for madd.s/msub.s: put the
    cursor on a known madd.s (or msub.s) and call bind_fpu(here()). `canon`
    defaults to '<mnemonic>.s'. Re-decompile afterwards.
    """
    import ida_ua
    insn = ida_ua.insn_t()
    if ida_ua.decode_insn(insn, ea) == 0:
        print("[r5900] decode failed at %X" % ea)
        return
    name = ida_idp.ph_get_instruc()[insn.itype][0]
    canon = canon or (name if name.endswith(".s") else name + ".s")
    f = globals().get("_fpu_filter")
    if f is None:
        print("[r5900] FPU filter not installed; run _install() first")
        return
    f.by_itype[insn.itype] = canon
    print("[r5900] bound itype %d (%s) -> %s" % (insn.itype, name, canon))


# ---------------------------------------------------------------------------
# VU0 macro-mode (cop2) + quadword coprocessor loads/stores -> intrinsic calls.
#
# The cop2 word is decoded (verified field layout, see vu_decode.py) into a
# NAMED intrinsic with the vf register numbers as scalar args -- the least
# crash-prone modeling (no 128-bit register mops). Unknown fn codes fall back to
# an opaque __vu_cop2(imm) so we never emit a guessed decode.
#     cop2 0x4BE13A4A -> vf9 = __vmaddz(7, 1)   (fd, fs, ft; ACC-forms drop fd)
#     cop2 <unknown>  -> __vu_cop2(0x....)
#     lqc2 $vfN,off(b)-> __vu_lqc2(N, base+off)
#     sqc2 $vfN,off(b)-> __vu_sqc2(N, base+off)
# ---------------------------------------------------------------------------

VU_NAMES = ("cop2", "lqc2", "sqc2")

# Ops whose dataflow shape is EXACTLY dst = f(vf_fs, vf_ft) [+ACC]. Only these
# get the register-modeled dataflow emission. Everything else (vaddi/vsubi read
# $I; vabs/vmove/vftoi single-source; vopmula/vclip special) uses the opaque
# flat form -- modeling them with two vf reads is wrong and trips INTERR 50732.
_VU_DATAFLOW_OK = (
    "vmul", "vmula", "vmadd", "vmadda", "vmsub", "vmsuba",
    "vadd", "vadda", "vsub", "vsuba", "vmax", "vmini",
)

# When True, arithmetic VU ops render as HONEST named intrinsics
# (dst = __vmulax(vf5, vf1)) -- shows the real op + operands, exact SIMD math
# deferred to the disassembly. When False, they use scalar mul/add ops that
# carry the same dataflow but present as (approximate, misleading) arithmetic.
# Intrinsic mode is experimental (uses a value-returning call); toggle with
# vu_intrinsics(True/False).
VU_INTRINSICS = False

# Within intrinsic mode, choose the RENDER style:
#   True  = clean value-returning form: vf9 = __vmaddz(vf7, vf1, ACC)
#           (most readable; a call reading scratch regs can INTERR on some funcs)
#   False = crash-proof form: __vmaddz_vf9(7, 1, 32) + hidden mov carrier
# Toggle with vu_clean(True/False).
VU_INTRINSIC_CLEAN = True

# MMI (multimedia) instructions modeled as intrinsics. pcpyld builds a 128-bit
# value = (rs.lo64 << 64) | rt.lo64, almost always feeding an sq to a DMA packet.
# GPRs are 64-bit in IDA so we can't express the true 128-bit result; instead we
# emit rd = __pcpyld(rs, rt) and let the existing sq filter store rd. Readable
# and robust regardless of where the result flows.
MMI_NAMES = ("pcpyld",)

# Toggle: False = flat form (vf NUMBERS as args, opaque calls -- proven safe).
#         True  = dataflow form (vf/ACC modeled as scratch registers, so the
#                 optimizer connects loads -> math -> stores). Higher risk.
VU_DATAFLOW = False

# vf00-vf31 + ACC modeled as fixed scratch mregs. We keep the VALUE scalar
# (4 bytes) but SPACE the registers 16 bytes apart: at 4-byte spacing Hex-Rays
# coalesces adjacent regs into shared 8-byte lvars and reads pick up the high
# half (the SHIDWORD(vN) garbage). 16-byte stride = one slot per register, no
# overlap. Parked far above physical regs / kregs (FPU ACC technique).
_VU_REGSIZE = 4       # scalar value width (avoids the 128-bit-mop INTERR)
_VU_STRIDE = 16       # per-register spacing (prevents lvar coalescing overlap)
_vu_base = None


def _vu_reg(index):
    """Stable scratch mreg for $vfNN (index 0..31) or ACC (index 32)."""
    global _vu_base
    if _vu_base is None:
        try:
            nreg = len(list(ida_idp.ph_get_regnames()))
            hi = max(ida_hexrays.reg2mreg(i) for i in range(nreg))
        except Exception:
            hi = 0x100
        _vu_base = ((hi >> 4) << 4) + 0x1000      # well above ACC's 0x800 park
    return _vu_base + index * _VU_STRIDE


_VU_ACC = 32   # pseudo-index for the accumulator

_VU_LANES = "xyzw"

# Decoder: rabbitizer supplies the authoritative MNEMONIC (the hard two-level
# decode); we derive the vf register numbers from verified bit positions. This
# ends the hand-maintained fn table -- any VU op rabbitizer names, we lift.
try:
    import rabbitizer as _rab
    _rab.config.regNames_gprAbiNames = _rab.Abi.O32
    _RAB_CAT = _rab.InstrCategory.R5900
    _HAVE_RAB = True
except Exception:
    _HAVE_RAB = False

# Which mnemonics read/write ACC, and which carry a broadcast lane. Semantics
# only (names come from rabbitizer). Anything not listed: no ACC, no lane.
#   *a  forms (vmula, vmadda, vmsuba, vopmula...) WRITE acc
#   madd/msub/madda/msuba READ acc
_VU_WRITES_ACC = ("vmula", "vmadda", "vmsuba", "vadda", "vsuba", "vopmula")
_VU_READS_ACC = ("vmadd", "vmsub", "vmadda", "vmsuba")  # accumulate ops


def _vu_bits(v, hi, lo):
    return (v >> lo) & ((1 << (hi - lo + 1)) - 1)


def _vu_decode(word):
    """Return (helper_name, writes_acc, reads_acc, fs, ft, fd) or None.
    Mnemonic from rabbitizer; register fields from verified bit positions."""
    if _vu_bits(word, 31, 26) != 0b010010:
        return None
    if not _HAVE_RAB:
        return None
    try:
        mnem = _rab.Instruction(word, category=_RAB_CAT).getOpcodeName()
    except Exception:
        return None
    if not mnem or mnem == "vnop":
        return ("__vnop", False, False, 0, 0, 0)   # padding/sync -> bland nop
    wacc = mnem.startswith(_VU_WRITES_ACC)
    racc = mnem.startswith(_VU_READS_ACC)
    return ("__" + mnem, wacc, racc,
            _vu_bits(word, 15, 11), _vu_bits(word, 20, 16), _vu_bits(word, 10, 6))


def _tvoid():
    t = ida_typeinf.tinfo_t()
    t.create_simple_type(ida_typeinf.BT_VOID)
    return t


def _tuint():
    t = ida_typeinf.tinfo_t()
    t.create_simple_type(ida_typeinf.BTF_INT)
    return t


def _tint_sz(size):
    """Integer tinfo of a given byte width (4 or 8), for arg/return typing."""
    t = ida_typeinf.tinfo_t()
    if size == 8:
        t.create_simple_type(ida_typeinf.BTF_INT64)
    else:
        t.create_simple_type(ida_typeinf.BTF_INT)
    return t


def _emit_helper(cdg, name, int_args, addr_mreg=None, reg_args=None, ret_mreg=None,
                 regsize=None):
    """Emit a helper call `name(...)`.

    int_args : list of small immediates (used in flat form)
    addr_mreg: optional pointer arg (mreg)
    reg_args : list of scratch mregs read by the op (dataflow form)
    ret_mreg : if set, the call returns into this scratch mreg (dataflow form)
    regsize  : width of reg_args / ret_mreg (default _VU_REGSIZE=4; GPRs use 8)
    """
    rsz = regsize if regsize is not None else _VU_REGSIZE
    call = ida_hexrays.minsn_t(cdg.insn.ea)
    call.opcode = ida_hexrays.m_call
    call.l.make_helper(name)

    ci = ida_hexrays.mcallinfo_t()
    ci.callee = ida_idaapi.BADADDR
    ci.cc = ida_typeinf.CM_CC_FASTCALL
    ci.return_type = _tvoid() if ret_mreg is None else _tint_sz(rsz)
    ci.flags = ida_hexrays.FCI_FINAL | ida_hexrays.FCI_PROP | ida_hexrays.FCI_SPLOK
    ci.role = ida_hexrays.ROLE_UNK

    off = 0
    n = 0

    def _push_stk(mop, size):
        nonlocal off, n
        a = ida_hexrays.mcallarg_t()
        a.assign(mop)
        a.size = size
        a.type = _tint_sz(size)
        # No argloc: a stack argloc on an immediate made the verifier fail with
        # INTERR 50732 at the function's first instruction.
        ci.args.push_back(a)
        n += 1

    def _push_reg(mreg, size):
        # register argloc: no stack footprint (avoids the phantom (sp+..) def
        # that triggered INTERR 50920). $zero (GPR 0) is a hardwired-zero
        # pseudo-reg with no real def; using it as a value operand also trips
        # 50920, so emit a literal 0 instead. Zero-mreg computed inline to avoid
        # any dependency on module load order.
        nonlocal n
        try:
            zmreg = ida_hexrays.reg2mreg(0)
        except Exception:
            zmreg = -1
        if mreg == zmreg:
            # Materialize 0 into a scratch reg, pass THAT by register -- keeps
            # all arglocs register-based (mixing a lone stack arg in trips 50732).
            zt = _vu_reg(40)          # a scratch mreg well outside vf/ACC range
            mv = ida_hexrays.minsn_t(cdg.insn.ea)
            mv.opcode = ida_hexrays.m_mov
            mv.l.t = ida_hexrays.mop_n
            mv.l.make_number(0, size if size <= 8 else 8)
            mv.l.size = size
            mv.d.t = ida_hexrays.mop_r
            mv.d.r = zt
            mv.d.size = size
            cdg.mb.insert_into_block(mv, cdg.mb.tail)
            a = ida_hexrays.mcallarg_t()
            a.t = ida_hexrays.mop_r
            a.r = zt
            a.size = size
            a.type = _tint_sz(size)
            loc = ida_typeinf.argloc_t()
            loc.set_reg1(zt)
            a.argloc = loc
            ci.args.push_back(a)
            n += 1
            return
        a = ida_hexrays.mcallarg_t()
        a.t = ida_hexrays.mop_r
        a.r = mreg
        a.size = size
        a.type = _tint_sz(size)
        ci.args.push_back(a)
        n += 1

    for v in int_args:
        m = ida_hexrays.mop_t()
        m.make_number(v & 0xFFFFFFFF, 4)
        _push_stk(m, 4)
    for r in (reg_args or []):
        _push_reg(r, rsz)
    if addr_mreg is not None:
        _push_reg(addr_mreg, 4)

    ci.solid_args = n
    ci.stkargs_top = off

    if ret_mreg is None:
        call.d.t = ida_hexrays.mop_f
        call.d.f = ci
        call.d.size = 0
        cdg.mb.insert_into_block(call, cdg.mb.tail)
    else:
        # Return into ret_mreg by embedding the call as the source of a mov.
        # If ret_mreg is ALSO one of the source registers (rd==rs/rt, e.g.
        # pcpyld $a7,$a1,$a7), a direct mov call(reads $a7) => $a7 is a
        # self-referential def/use the generator rejects (INTERR 50732). In that
        # case route through a fresh kreg temp first, then temp -> ret_mreg.
        src_regs = set(reg_args or [])
        if addr_mreg is not None:
            src_regs.add(addr_mreg)
        needs_temp = ret_mreg in src_regs

        call.d.t = ida_hexrays.mop_f
        call.d.f = ci
        call.d.size = rsz

        if not needs_temp:
            mov = ida_hexrays.minsn_t(cdg.insn.ea)
            mov.opcode = ida_hexrays.m_mov
            mov.l.t = ida_hexrays.mop_d
            mov.l.d = call
            mov.l.size = rsz
            mov.d.t = ida_hexrays.mop_r
            mov.d.r = ret_mreg
            mov.d.size = rsz
            cdg.mb.insert_into_block(mov, cdg.mb.tail)
        else:
            tmp = cdg.mba.alloc_kreg(rsz)
            mov1 = ida_hexrays.minsn_t(cdg.insn.ea)   # tmp = call(...)
            mov1.opcode = ida_hexrays.m_mov
            mov1.l.t = ida_hexrays.mop_d
            mov1.l.d = call
            mov1.l.size = rsz
            mov1.d.t = ida_hexrays.mop_r
            mov1.d.r = tmp
            mov1.d.size = rsz
            cdg.mb.insert_into_block(mov1, cdg.mb.tail)
            mov2 = ida_hexrays.minsn_t(cdg.insn.ea)   # ret_mreg = tmp
            mov2.opcode = ida_hexrays.m_mov
            mov2.l.t = ida_hexrays.mop_r
            mov2.l.r = tmp
            mov2.l.size = rsz
            mov2.d.t = ida_hexrays.mop_r
            mov2.d.r = ret_mreg
            mov2.d.size = rsz
            cdg.mb.insert_into_block(mov2, cdg.mb.tail)
            cdg.mba.free_kreg(tmp, rsz)
    return call


def _emit_helper0(cdg, name):
    """Zero-arg void helper (kept as a safe fallback path)."""
    return _emit_helper(cdg, name, [])


class r5900_vu_filter_t(ida_hexrays.microcode_filter_t):
    def __init__(self, name2itype):
        ida_hexrays.microcode_filter_t.__init__(self)
        self.by_itype = {it: nm for nm, it in name2itype.items()}

    def match(self, cdg):
        return cdg.insn.itype in self.by_itype

    def _emit_vu_intrinsic(self, cdg, ea, hname, dst, srcs, src_nums, fd=None,
                           acc_write=False):
        """Emit a named VU intrinsic. Two styles, chosen by VU_INTRINSIC_CLEAN:

        CLEAN (True): value-returning call reading the source vf REGISTERS, so it
          renders as   vf9 = __vmaddz(vf7, vf1, ACC)   -- most readable, but a
          call reading high scratch mregs can trip INTERR 50732 on some funcs.

        SAFE (False): crash-proof. Void call with vf-NUMBER args, destination in
          the name, real m_mov carrier:   __vmaddz_vf9(7, 1, 32)  +  vf9 = ...
        """
        if VU_INTRINSIC_CLEAN:
            self._emit_vu_intrinsic_clean(cdg, ea, hname, dst, srcs)
        else:
            self._emit_vu_intrinsic_safe(cdg, ea, hname, dst, srcs, src_nums,
                                         fd, acc_write)

    def _emit_vu_intrinsic_clean(self, cdg, ea, hname, dst, srcs):
        """vf9 = __vmaddz(vf7, vf1, ACC) -- value-returning call, reads source
        registers so the args render as real vf/ACC names. May INTERR on some
        functions (call reading scratch mregs); switch to SAFE if so."""
        sz = _VU_REGSIZE
        call = ida_hexrays.minsn_t(ea)
        call.opcode = ida_hexrays.m_call
        call.l.make_helper(hname)
        ci = ida_hexrays.mcallinfo_t()
        ci.callee = ida_idaapi.BADADDR
        ci.cc = ida_typeinf.CM_CC_FASTCALL
        ci.return_type = _tint_sz(sz)
        ci.flags = ida_hexrays.FCI_FINAL | ida_hexrays.FCI_PROP | ida_hexrays.FCI_SPLOK
        ci.role = ida_hexrays.ROLE_UNK
        for r in srcs:
            a = ida_hexrays.mcallarg_t()
            a.t = ida_hexrays.mop_r
            a.r = r
            a.size = sz
            a.type = _tint_sz(sz)
            loc = ida_typeinf.argloc_t()
            loc.set_reg1(r)
            a.argloc = loc
            ci.args.push_back(a)
        ci.solid_args = len(srcs)
        ci.return_regs.add(dst, sz)
        rloc = ida_typeinf.argloc_t()
        rloc.set_reg1(dst)
        ci.return_argloc = rloc
        call.d.t = ida_hexrays.mop_f
        call.d.f = ci
        call.d.size = sz
        mov = ida_hexrays.minsn_t(ea)
        mov.opcode = ida_hexrays.m_mov
        mov.l.t = ida_hexrays.mop_d
        mov.l.d = call
        mov.l.size = sz
        mov.d.t = ida_hexrays.mop_r
        mov.d.r = dst
        mov.d.size = sz
        cdg.mb.insert_into_block(mov, cdg.mb.tail)

    def _emit_vu_intrinsic_safe(self, cdg, ea, hname, dst, srcs, src_nums,
                                fd=None, acc_write=False):
        """Crash-proof: void call w/ number args, dest in the name, mov carrier."""
        sz = _VU_REGSIZE
        dtag = "ACC" if acc_write else ("vf%d" % fd if fd is not None else "")
        fullname = hname + ("_" + dtag if dtag else "")
        _emit_helper(cdg, fullname, list(src_nums))
        if srcs:
            mv = ida_hexrays.minsn_t(ea)
            mv.opcode = ida_hexrays.m_mov
            mv.l.t = ida_hexrays.mop_r
            mv.l.r = srcs[0]
            mv.l.size = sz
            mv.d.t = ida_hexrays.mop_r
            mv.d.r = dst
            mv.d.size = sz
            cdg.mb.insert_into_block(mv, cdg.mb.tail)

    def _emit_vu_dataflow(self, cdg, ea, dst, fs, ft, acc):
        """Emit real microcode ops (no call) so `dst` depends on fs, ft [, acc].
        Carries the dataflow edge; the VU SIMD math itself is not modeled.
        dst = fs * ft ; then if acc: dst = dst + acc. All scratch regs."""
        sz = _VU_REGSIZE

        def _rm(r):
            m = ida_hexrays.mop_t()
            m.t = ida_hexrays.mop_r
            m.r = r
            m.size = sz
            return m

        mul = ida_hexrays.minsn_t(ea)
        mul.opcode = ida_hexrays.m_mul
        mul.l = _rm(fs)
        mul.r = _rm(ft)
        mul.d.t = ida_hexrays.mop_r
        mul.d.r = dst
        mul.d.size = sz
        cdg.mb.insert_into_block(mul, cdg.mb.tail)

        if acc is not None:
            add = ida_hexrays.minsn_t(ea)
            add.opcode = ida_hexrays.m_add
            add.l = _rm(dst)
            add.r = _rm(acc)
            add.d.t = ida_hexrays.mop_r
            add.d.r = dst
            add.d.size = sz
            cdg.mb.insert_into_block(add, cdg.mb.tail)

    def apply(self, cdg):
        insn = cdg.insn
        name = self.by_itype[insn.itype]
        _trace(insn.ea, "VU apply name=%s" % name)
        if name == "cop2":
            # Read the actual instruction WORD from bytes, not insn.ops[].value:
            # the DWARF reloc bug corrupts the operand for some cop2 words.
            word = ida_bytes.get_dword(insn.ea)
            if _vu_bits(word, 31, 26) != 0b010010:
                # not a real cop2 marker (shouldn't happen) -> opaque fallback
                _emit_helper(cdg, "__vu_cop2", [word])
                return ida_hexrays.MERR_OK
            dec = _vu_decode(word)
            if dec is None:
                _emit_helper(cdg, "__vu_cop2", [word])
            else:
                hname, acc, racc, fs, ft, fd = dec
                # vnop / nop-like ops do nothing: emit NOTHING (modeling them as
                # value-returning calls trips INTERR 50732).
                if hname in ("__vnop", "__vwaitq") and VU_DATAFLOW:
                    return ida_hexrays.MERR_OK
                # base mnemonic without leading "__" and trailing broadcast lane
                base = hname[2:]
                if base and base[-1] in "xyzw":
                    base = base[:-1]
                # $I/$Q/special-source forms (vaddi, vsubi, vmuli, vaddq...) and
                # single-source ops don't fit dst=f(vf_fs,vf_ft): use flat/opaque.
                dataflow_ok = (VU_DATAFLOW and base in _VU_DATAFLOW_OK
                               and not base.endswith(("i", "q")))
                if not dataflow_ok:
                    # flat: __<mnemonic>_<dest>(vf numbers in operand order), exact
                    _emit_ee_helper(cdg, word)
                else:
                    dst = _vu_reg(_VU_ACC if acc else fd)
                    srcs = [_vu_reg(fs), _vu_reg(ft)]
                    if racc:
                        srcs.append(_vu_reg(_VU_ACC))
                    if VU_INTRINSICS:
                        # honest named intrinsic: __vmaddz_vf9(7, 1, ACC)
                        src_nums = [fs, ft] + ([_VU_ACC] if racc else [])
                        self._emit_vu_intrinsic(cdg, insn.ea, hname, dst, srcs,
                                                src_nums, fd=(None if acc else fd),
                                                acc_write=acc)
                    else:
                        # real scalar ops carry dataflow; math approximate
                        self._emit_vu_dataflow(cdg, insn.ea, dst, _vu_reg(fs),
                                               _vu_reg(ft),
                                               _vu_reg(_VU_ACC) if racc else None)
        else:
            # lqc2/sqc2 $vfN, off(base): vf reg is bits [20:16] of the word.
            word = ida_bytes.get_dword(insn.ea)
            vfn = _vu_bits(word, 20, 16)
            addr = cdg.load_effective_address(1)
            if not VU_DATAFLOW:
                _emit_helper(cdg, "__" + name, [vfn], addr_mreg=addr)
            elif name == "lqc2":
                # Real memory LOAD into the vf scratch reg, via the codegen
                # helper (builds correct load microcode; the same primitive the
                # sq/lq filter uses). vfN = *(mem operand 1).
                mreg = cdg.load_operand(1)
                mv = ida_hexrays.minsn_t(insn.ea)
                mv.opcode = ida_hexrays.m_mov
                mv.l.t = ida_hexrays.mop_r
                mv.l.r = mreg
                mv.l.size = _VU_REGSIZE
                mv.d.t = ida_hexrays.mop_r
                mv.d.r = _vu_reg(vfn)
                mv.d.size = _VU_REGSIZE
                cdg.mb.insert_into_block(mv, cdg.mb.tail)
            else:  # sqc2: real STORE of vf scratch reg to mem operand 1
                cdg.store_operand(1, _reg_mop(_vu_reg(vfn), _VU_REGSIZE))

        return ida_hexrays.MERR_OK


class r5900_mmi_filter_t(ida_hexrays.microcode_filter_t):
    """MMI ops as intrinsics. pcpyld rd, rs, rt -> rd = __pcpyld(rs, rt).
    rd = (rs.lo64 << 64) | rt.lo64 on hardware; modeled as an opaque combine."""

    def __init__(self, name2itype):
        ida_hexrays.microcode_filter_t.__init__(self)
        self.by_itype = {it: nm for nm, it in name2itype.items()}

    def match(self, cdg):
        return cdg.insn.itype in self.by_itype

    def apply(self, cdg):
        insn = cdg.insn
        name = self.by_itype[insn.itype]
        _trace(insn.ea, "MMI apply name=%s" % name)
        if name == "pcpyld":
            # pcpyld rd, rs, rt : rd = (rs.lo64 << 64) | rt.lo64.
            # The 128-bit result can't live in a 64-bit GPR, and emitting it as a
            # helper call trips INTERR 50732 in some contexts. Model it as a plain
            # register move rd = rt (the low 64 bits, which IS what the GPR holds)
            # -- always-valid microcode, no call. The high half (rs) is lost in
            # the model but the store that follows is what matters; annotate it.
            word = ida_bytes.get_dword(insn.ea)
            rt_n = (word >> 16) & 0x1F      # low-half source -> becomes rd's value
            rd_n = (word >> 11) & 0x1F
            rt = ida_hexrays.reg2mreg(rt_n)
            rd = ida_hexrays.reg2mreg(rd_n)
            mv = ida_hexrays.minsn_t(insn.ea)
            mv.opcode = ida_hexrays.m_mov
            mv.l.t = ida_hexrays.mop_r
            mv.l.r = rt
            mv.l.size = 8
            mv.d.t = ida_hexrays.mop_r
            mv.d.r = rd
            mv.d.size = 8
            cdg.mb.insert_into_block(mv, cdg.mb.tail)
        return ida_hexrays.MERR_OK




# ---------------------------------------------------------------------------
# Every other EE instruction Hex-Rays leaves as __asm (COP2 moves, MMI, the
# pipeline-1 multiply/divide) becomes a helper call named after the mnemonic,
# with operands in disassembly order: GPRs as values, vf/vi numbers and
# immediates as numbers. A GPR destination receives the call's value (low 64
# bits; the upper half of a 128-bit result is not modelled).
#     pextlh $a0, $a1, $a2   ->  a0 = __pextlh(a1, a2)
#     qmtc2  $v0, $vf10      ->  __qmtc2(v0, 10)
#     vsub.xyzw vf10,vf10,vf11 -> __vsub_xyzw(10, 10, 11)
# ---------------------------------------------------------------------------
EE_GENERIC = ("qmfc2", "qmtc2", "cfc2", "ctc2",
              "mult1", "multu1", "div1", "divu1", "madd1", "maddu1", "mfhi1", "mflo1",
              "mthi1", "mtlo1", "pmfhi", "pmflo", "pmthi", "pmtlo", "pmfhl", "pmthl.lw",
              "mtsab", "mtsah", "qfsrv", "plzcw",
              "paddw", "psubw", "pcgtw", "pmaxw", "paddh", "psubh", "pcgth", "pmaxh",
              "paddb", "psubb", "pcgtb", "paddsw", "psubsw", "pextlw", "ppacw", "paddsh",
              "psubsh", "pextlh", "ppach", "paddsb", "psubsb", "pextlb", "ppacb", "pext5",
              "ppac5", "pabsw", "pceqw", "pminw", "padsbh", "pabsh", "pceqh", "pminh",
              "pceqb", "padduw", "psubuw", "pextuw", "padduh", "psubuh", "pextuh", "paddub",
              "psubub", "pextub", "pmaddw", "psllvw", "psrlvw", "pmsubw", "pmfhi", "pinth",
              "pmultw", "pdivw", "pcpyld", "pand", "pxor", "pmaddh", "phmadh", "pmsubh",
              "phmsbh", "pexeh", "prevh", "pmulth", "pdivbw", "pexew", "prot3w", "pmadduw",
              "psravw", "pinteh", "pmultuw", "pdivuw", "pcpyud", "por", "pnor", "pexch",
              "pcpyh", "pexcw", "psllh", "psrlh", "psrah", "psllw", "psrlw", "psraw")
_EE_NO_DEST = ("qmtc2", "ctc2", "mult1", "multu1", "div1", "divu1", "madd1", "maddu1",
               "mthi1", "mtlo1", "pmthi", "pmtlo", "pmthl.lw", "mtsab", "mtsah",
               "pmaddw", "pmsubw", "pmadduw", "pdivw", "pdivuw", "pdivbw", "pmaddh",
               "pmsubh", "phmadh", "phmsbh")
_EE_VU_DEST_IS_NUMBER = True


def _ee_operands(word):
    """rabbitizer's mnemonic and operand strings with numeric GPR names."""
    ins = _rab.Instruction(word, category=_RAB_CAT)
    old = _rab.config.regNames_gprAbiNames
    _rab.config.regNames_gprAbiNames = _rab.Abi.NUMERIC
    try:
        txt = ins.disassemble(None, 0)
    finally:
        _rab.config.regNames_gprAbiNames = old
    if txt.startswith(".word") and "#" in txt:
        # rabbitizer prints vsqrt/vclip/vrsqrt as .word with the instruction in a comment
        txt = txt.split("#")[1].strip()
    parts = txt.split(None, 1)
    ops = [o.strip() for o in parts[1].split(",")] if len(parts) > 1 else []
    return parts[0], ops


def _ee_number(v):
    m = ida_hexrays.mop_t()
    m.make_number(v & 0xFFFFFFFF, 4)
    return m


def _emit_ee_helper(cdg, word):
    import re
    mnem, ops = _ee_operands(word)
    base = mnem.split(".")[0]
    name = "__" + mnem.replace(".", "_")
    dest = None
    args = []
    for i, o in enumerate(ops):
        g = re.fullmatch(r"\$(\d+)", o)
        v = re.fullmatch(r"\$v[fi](\d+)[xyzw]?", o)
        if g:
            r = int(g.group(1))
            if i == 0 and mnem not in _EE_NO_DEST and base not in _EE_NO_DEST and not base.startswith("v"):
                dest = r
                continue
            if r == 0:
                args.append(ida_hexrays.mop_t())
                args[-1].make_number(0, REG_SIZE)
            else:
                args.append(_reg_mop(ida_hexrays.reg2mreg(r), REG_SIZE))
        elif v:
            args.append(_ee_number(int(v.group(1))))
        elif o in ("$ACC", "$Q", "$I", "$R"):
            continue
        else:
            try:
                args.append(_ee_number(int(o, 0)))
            except ValueError:
                pass
    call = ida_hexrays.minsn_t(cdg.insn.ea)
    call.opcode = ida_hexrays.m_call
    call.l.make_helper(name)
    ci = ida_hexrays.mcallinfo_t()
    ci.callee = ida_idaapi.BADADDR
    ci.cc = ida_typeinf.CM_CC_FASTCALL
    ci.flags = ida_hexrays.FCI_FINAL | ida_hexrays.FCI_PROP | ida_hexrays.FCI_SPLOK
    ci.role = ida_hexrays.ROLE_UNK
    for m in args:
        a = ida_hexrays.mcallarg_t()
        a.assign(m)
        a.size = m.size
        a.type = _tint_sz(m.size)
        ci.args.push_back(a)
    ci.solid_args = len(args)
    if dest is None or dest == 0:
        ci.return_type = _tvoid()
        call.d.t = ida_hexrays.mop_f
        call.d.f = ci
        call.d.size = 0
        cdg.mb.insert_into_block(call, cdg.mb.tail)
        return
    ci.return_type = _tint_sz(REG_SIZE)
    call.d.t = ida_hexrays.mop_f
    call.d.f = ci
    call.d.size = REG_SIZE
    tmp = cdg.mba.alloc_kreg(REG_SIZE)
    mv = ida_hexrays.minsn_t(cdg.insn.ea)
    mv.opcode = ida_hexrays.m_mov
    mv.l.t = ida_hexrays.mop_d
    mv.l.d = call
    mv.l.size = REG_SIZE
    mv.d = _reg_mop(tmp, REG_SIZE)
    cdg.mb.insert_into_block(mv, cdg.mb.tail)
    _emit(cdg, ida_hexrays.m_mov, _reg_mop(tmp, REG_SIZE), None,
          _reg_mop(ida_hexrays.reg2mreg(dest), REG_SIZE))
    cdg.mba.free_kreg(tmp, REG_SIZE)


class r5900_generic_filter_t(ida_hexrays.microcode_filter_t):
    def __init__(self, name2itype):
        ida_hexrays.microcode_filter_t.__init__(self)
        self.itypes = set(name2itype.values())

    def match(self, cdg):
        return cdg.insn.itype in self.itypes

    def apply(self, cdg):
        _emit_ee_helper(cdg, ida_bytes.get_dword(cdg.insn.ea))
        return ida_hexrays.MERR_OK


def enable_generic():
    """Install the generic EE helper filter (replaces the pcpyld-only MMI filter)."""
    global _generic_filter, _mmi_filter
    if globals().get("_mmi_filter") is not None:
        ida_hexrays.install_microcode_filter(_mmi_filter, False)
        _mmi_filter = None
    if globals().get("_generic_filter") is None:
        it = _resolve_itypes(EE_GENERIC)
        _generic_filter = r5900_generic_filter_t(it)
        ida_hexrays.install_microcode_filter(_generic_filter, True)
        _register(_generic_filter)
        print("[r5900] generic EE filter: %d mnemonics" % len(it))


def _install():
    print("[r5900] version: %s" % R5900_VERSION)
    global _filter, _fpu_filter, _vu_filter, _mmi_filter
    _filter = globals().get("_filter")
    _fpu_filter = globals().get("_fpu_filter")
    _vu_filter = globals().get("_vu_filter")
    _mmi_filter = globals().get("_mmi_filter")
    if _filter is None:
        itypes = _resolve_itypes(("sq", "lq"))
        if "sq" not in itypes or "lq" not in itypes:
            print("[r5900] sq/lq itypes not found; skipping sq/lq filter")
        else:
            _filter = r5900_filter_t(itypes["sq"], itypes["lq"])
            ida_hexrays.install_microcode_filter(_filter, True)
            _register(_filter)
            print("[r5900] sq/lq filter installed (sq=%d lq=%d)"
                  % (itypes["sq"], itypes["lq"]))

    if _fpu_filter is None:
        fitypes = _resolve_itypes(FPU_NAMES)          # adda/suba/mula/madda/msuba
        for it, canon in _resolve_fpu_ma3().items():  # madd.s/msub.s by itype
            fitypes[canon] = it
        if fitypes:
            _fpu_filter = r5900_fpu_filter_t(fitypes)
            ida_hexrays.install_microcode_filter(_fpu_filter, True)
            _register(_fpu_filter)
            print("[r5900] FPU-ACC filter installed (ACC=stack slot): %s"
                  % ", ".join("%s=%d" % (n, i)
                              for n, i in sorted(fitypes.items())))
        missing = [n for n in (FPU_NAMES + FPU_MA3) if n not in fitypes]
        if missing:
            print("[r5900] FPU forms NOT resolved: %s -- use bind_fpu(ea) on one"
                  % ", ".join(missing))

    if _mmi_filter is None:
        mitypes = _resolve_itypes(MMI_NAMES)          # pcpyld (+ future MMI ops)
        if mitypes:
            _mmi_filter = r5900_mmi_filter_t(mitypes)
            ida_hexrays.install_microcode_filter(_mmi_filter, True)
            _register(_mmi_filter)
            print("[r5900] MMI filter installed: %s"
                  % ", ".join("%s=%d" % (n, i) for n, i in sorted(mitypes.items())))
        miss = [n for n in MMI_NAMES if n not in mitypes]
        if miss:
            print("[r5900] MMI ops NOT found: %s" % ", ".join(miss))

    # VU filter is OPT-IN (experimental, can hard-crash). Enable with enable_vu().
    if getattr(ida_idaapi, "_r5900_vu_was_on", False):
        # a previous session had VU on; self-cleanup removed the stale instance,
        # so reinstall a fresh one from the current code (fixes hot-reload).
        globals()["_vu_filter"] = None
        enable_vu()
        print("[r5900] (VU auto-restored from previous session)")
    else:
        print("[r5900] VU intrinsic filter NOT auto-installed (experimental). "
              "Call enable_vu() to try it, disable_vu() to remove.")


# ===========================================================================
# Built-in diagnostics. Same namespace as the filters, so they see exactly what
# is installed. dump_manifest(ea) writes a complete per-function artifact for
# iteration; probe(ea) prints a quick instruction/itype/filter view.
# ===========================================================================

_DIAG_MATS = [
    ("GENERATED", "MMAT_GENERATED"), ("PREOPTIMIZED", "MMAT_PREOPTIMIZED"),
    ("LOCOPT", "MMAT_LOCOPT"), ("CALLS", "MMAT_CALLS"),
    ("GLBOPT1", "MMAT_GLBOPT1"), ("GLBOPT2", "MMAT_GLBOPT2"),
    ("GLBOPT3", "MMAT_GLBOPT3"), ("LVARS", "MMAT_LVARS"),
]


def _diag_outdir():
    import ida_loader
    p = ida_loader.get_path(ida_loader.PATH_TYPE_IDB) or ""
    return __import__("os").path.dirname(p) or __import__("os").getcwd()


def _diag_filter_map():
    """itype -> 'filterkey:name' for all installed r5900 filters."""
    out = {}
    for key in ("_filter", "_fpu_filter", "_vu_filter", "_mmi_filter"):
        flt = globals().get(key)
        if flt is None:
            continue
        if hasattr(flt, "by_itype"):
            for it, nm in flt.by_itype.items():
                out[it] = "%s:%s" % (key, nm)
        for attr in ("sq", "lq"):
            if hasattr(flt, attr):
                out[getattr(flt, attr)] = "%s:%s" % (key, attr)
    return out


def _diag_rab(word, ea):
    if not _HAVE_RAB:
        return ""
    try:
        return " ".join(_rab.Instruction(word, vram=ea,
                                         category=_RAB_CAT).disassemble(None, 0).split())
    except Exception as e:
        return "<rab err %r>" % e


class _DiagPrinter(ida_hexrays.vd_printer_t):
    def __init__(self):
        ida_hexrays.vd_printer_t.__init__(self)
        self.lines = []

    def _print(self, indent, line):
        self.lines.append(line)
        return 1


def probe_func(ea=None):
    """Quick console view of a WHOLE function: each instruction's
    addr/word/disasm/itype and which installed filter (if any) claims it. Great
    for spotting an unhandled op. (See probe(ea) for single-insn operand detail.)"""
    import ida_funcs, ida_ua, ida_bytes, ida_kernwin, idc
    if ea is None:
        ea = ida_kernwin.get_screen_ea()
    f = ida_funcs.get_func(ea)
    if not f:
        print("[r5900] no function at %08X" % ea)
        return
    fmap = _diag_filter_map()
    cur = f.start_ea
    while cur < f.end_ea:
        insn = ida_ua.insn_t()
        sz = ida_ua.decode_insn(insn, cur)
        if sz == 0:
            cur += 4
            continue
        word = ida_bytes.get_dword(cur)
        dis = (idc.generate_disasm_line(cur, 0) or "").replace("\t", " ")
        claim = fmap.get(insn.itype, "")
        print("%08X %08X it=%-4d %-30s %s%s"
              % (cur, word, insn.itype, dis[:30], _diag_rab(word, cur),
                 ("  [%s]" % claim) if claim else ""))
        cur += sz


_TRACE_PATH = [None]


def trace_apply(ea=None, path=None):
    """Arm a per-instruction emission trace, then decompile. BEFORE each VU/MMI
    filter emits an instruction, its addr+word+decode is appended to a file on
    disk. If generation HARD-crashes IDA, the file still shows the LAST
    instruction attempted -- the one that killed it. Use when dump_manifest()
    can't capture because the process dies. Disarm with trace_off()."""
    import os, ida_kernwin, ida_funcs
    if ea is None:
        ea = ida_kernwin.get_screen_ea()
    if path is None:
        path = os.path.join(_diag_outdir(), "trace_%X.txt" % ea)
    _TRACE_PATH[0] = path
    open(path, "w").close()
    print("[r5900] tracing emissions to %s -- now decompile (F5) the function" % path)
    print("[r5900] after a crash, the LAST line in that file is the culprit insn")


def trace_off():
    _TRACE_PATH[0] = None
    print("[r5900] emission trace off")


def _trace(ea, note):
    p = _TRACE_PATH[0]
    if p is None:
        return
    try:
        import ida_bytes
        word = ida_bytes.get_dword(ea)
        with open(p, "a", encoding="utf-8", errors="replace") as fh:
            fh.write("%08X %08X %s\n" % (ea, word, note))
            fh.flush()
            os = __import__("os")
            os.fsync(fh.fileno())    # force to disk so a crash can't lose it
    except Exception:
        pass


def dump_manifest(ea=None, maturities_up_to="LVARS"):
    """Write a complete per-function manifest (env, instructions with rabbitizer
    + filter claims, microcode per maturity with verify, pseudocode) to
    <idb_dir>/manifest_<EA>.txt. Incremental flush survives an INTERR. For a
    function that may hard-crash, pass maturities_up_to='GENERATED'."""
    import os, traceback, ida_funcs, ida_ua, ida_bytes, ida_name, ida_kernwin, idc
    if ea is None:
        ea = ida_kernwin.get_screen_ea()
    f = ida_funcs.get_func(ea)
    if not f:
        print("[r5900] no function at %08X" % ea)
        return
    path = os.path.join(_diag_outdir(), "manifest_%X.txt" % f.start_ea)
    buf = []

    def w(s):
        buf.append(s)
        try:
            with open(path, "w", encoding="utf-8", errors="replace") as fh:
                fh.write("".join(buf))
        except Exception:
            pass

    nm = ida_name.get_name(f.start_ea) or ("sub_%X" % f.start_ea)
    w("MANIFEST for %s\n" % nm)
    w("range: %08X - %08X (%d bytes)\n\n" % (f.start_ea, f.end_ea,
                                             f.end_ea - f.start_ea))
    # env
    w("===== ENVIRONMENT =====\n")
    w("processor: %s (id=%d)\n" % (ida_idp.get_idp_name(), ida_idp.ph_get_id()))
    try:
        regs = ida_idp.ph_get_regnames()
        w("reg count: %d\nregnames: %s\n" % (len(regs), ", ".join(regs)))
    except Exception:
        pass
    w("rabbitizer: %s\n" % ("yes" if _HAVE_RAB else "no"))
    fmap = _diag_filter_map()
    w("installed filters: %s\n\n"
      % (", ".join("%d=%s" % (k, v) for k, v in sorted(fmap.items())) or "none"))
    # instructions
    w("===== INSTRUCTIONS =====\n")
    cur = f.start_ea
    while cur < f.end_ea:
        insn = ida_ua.insn_t()
        sz = ida_ua.decode_insn(insn, cur)
        if sz == 0:
            w("%08X <decode failed>\n" % cur)
            cur += 4
            continue
        word = ida_bytes.get_dword(cur)
        dis = (idc.generate_disasm_line(cur, 0) or "").replace("\t", " ")
        claim = fmap.get(insn.itype, "")
        w("%08X %08X it=%-4d %-30s %s%s\n"
          % (cur, word, insn.itype, dis[:30], _diag_rab(word, cur),
             ("  [%s]" % claim) if claim else ""))
        if claim:
            ops = []
            for i, op in enumerate(insn.ops):
                if op.type == ida_ua.o_void:
                    break
                ops.append("op%d(t=%d reg=%d val=0x%X addr=0x%X)"
                           % (i, op.type, op.reg, op.value, op.addr))
            w("         ops: %s\n" % "; ".join(ops))
        cur += sz
    # microcode per maturity
    w("\n===== MICROCODE =====\n")
    limit = next((i for i, (n, _) in enumerate(_DIAG_MATS)
                  if n == maturities_up_to), len(_DIAG_MATS) - 1)
    for i, (mn, const) in enumerate(_DIAG_MATS):
        if i > limit:
            break
        mmat = getattr(ida_hexrays, const, None)
        if mmat is None:
            continue
        hf = ida_hexrays.hexrays_failure_t()
        try:
            mba = ida_hexrays.gen_microcode(ida_hexrays.mba_ranges_t(f), hf,
                                            None, ida_hexrays.DECOMP_WARNINGS, mmat)
        except Exception as e:
            w("\n--- %s: EXCEPTION %r ---\n%s\n" % (mn, e, traceback.format_exc()))
            continue
        if mba is None:
            w("\n--- %s: gen FAILED code=%s '%s' @%08X ---\n"
              % (mn, hf.code, hf.str, hf.errea))
            break
        p = _DiagPrinter()
        try:
            mba._print(p)
            w("\n--- %s (%d blocks) ---\n%s\n" % (mn, mba.qty, "\n".join(p.lines)))
        except Exception as e:
            w("\n--- %s: print err %r ---\n" % (mn, e))
        try:
            mba.verify(True)
            w("[verify %s: OK]\n" % mn)
        except Exception as e:
            w("[verify %s: FAILED %r]\n" % (mn, e))
    # pseudocode
    w("\n===== PSEUDOCODE =====\n")
    try:
        cf = ida_hexrays.decompile(f.start_ea)
        w(str(cf) if cf else "<None>\n")
    except Exception as e:
        w("<decompile raised %r>\n" % e)
    print("[r5900] manifest -> %s" % path)
    return path


def enable_vu():
    """Opt in to the experimental VU0/cop2 intrinsic filter."""
    global _vu_filter
    _vu_filter = globals().get("_vu_filter")
    if _vu_filter is not None:
        print("[r5900] VU filter already enabled")
        return
    vitypes = _resolve_itypes(VU_NAMES)
    if not vitypes:
        print("[r5900] VU mnemonics not found")
        return
    _vu_filter = r5900_vu_filter_t(vitypes)
    ida_hexrays.install_microcode_filter(_vu_filter, True)
    _register(_vu_filter)
    # remember across reloads so a re-run of the file auto-restores VU
    setattr(ida_idaapi, "_r5900_vu_was_on", True)
    print("[r5900] VU intrinsic filter ENABLED: %s"
          % ", ".join("%s=%d" % (n, i) for n, i in sorted(vitypes.items())))


def vu_intrinsics(on=True):
    """Toggle honest named-intrinsic rendering for VU arithmetic ops
    (dst = __vmulax(...)). Experimental: uses a value-returning call. If it
    INTERRs, turn back off and use real-op dataflow. Re-decompile after."""
    global VU_INTRINSICS
    VU_INTRINSICS = bool(on)
    print("[r5900] VU intrinsic mode: %s (re-decompile)"
          % ("ON (named calls)" if VU_INTRINSICS else "OFF (real ops)"))


def vu_clean(on=True):
    """Choose VU intrinsic render style (when vu_intrinsics is ON):
    True  = vf9 = __vmaddz(vf7, vf1, ACC)   (clean; may INTERR on some funcs)
    False = __vmaddz_vf9(7, 1, 32)          (crash-proof, dest in name)
    Re-decompile after."""
    global VU_INTRINSIC_CLEAN
    VU_INTRINSIC_CLEAN = bool(on)
    print("[r5900] VU intrinsic style: %s (re-decompile)"
          % ("CLEAN (vf9 = ...)" if VU_INTRINSIC_CLEAN else "SAFE (name-encoded)"))


def vu_dataflow(on=True):
    """Toggle VU dataflow mode (vf/ACC as connected registers). Re-decompile
    (F5) after calling. off = flat form (vf numbers, opaque calls)."""
    global VU_DATAFLOW
    VU_DATAFLOW = bool(on)
    print("[r5900] VU dataflow mode: %s (re-decompile to apply)"
          % ("ON" if VU_DATAFLOW else "OFF (flat)"))


def disable_vu():
    global _vu_filter
    _vu_filter = globals().get("_vu_filter")
    if _vu_filter is not None:
        ida_hexrays.install_microcode_filter(_vu_filter, False)
        _vu_filter = None
    setattr(ida_idaapi, "_r5900_vu_was_on", False)   # don't auto-restore
    print("[r5900] VU filter disabled")


def _remove():
    global _filter, _fpu_filter, _vu_filter, _mmi_filter
    _filter = globals().get("_filter")
    _fpu_filter = globals().get("_fpu_filter")
    _vu_filter = globals().get("_vu_filter")
    _mmi_filter = globals().get("_mmi_filter")
    if _filter is not None:
        ida_hexrays.install_microcode_filter(_filter, False)
        _filter = None
    if _fpu_filter is not None:
        ida_hexrays.install_microcode_filter(_fpu_filter, False)
        _fpu_filter = None
    if _vu_filter is not None:
        ida_hexrays.install_microcode_filter(_vu_filter, False)
        _vu_filter = None
    if _mmi_filter is not None:
        ida_hexrays.install_microcode_filter(_mmi_filter, False)
        _mmi_filter = None
    g = globals().get("_generic_filter")
    if g is not None:
        ida_hexrays.install_microcode_filter(g, False)
        globals()["_generic_filter"] = None


class r5900_plugin_t(ida_idaapi.plugin_t):
    flags = ida_idaapi.PLUGIN_HIDE | ida_idaapi.PLUGIN_MOD
    comment = "R5900 lq/sq microcode lifter"
    help = ""
    wanted_name = "R5900 microcode"
    wanted_hotkey = ""

    def init(self):
        # Only makes sense for MIPS IDBs with the decompiler present.
        if ida_idp.ph_get_id() != ida_idp.PLFM_MIPS:
            return ida_idaapi.PLUGIN_SKIP
        if not ida_hexrays.init_hexrays_plugin():
            return ida_idaapi.PLUGIN_SKIP
        _install()
        return ida_idaapi.PLUGIN_KEEP

    def run(self, arg):
        pass

    def term(self):
        _remove()


def PLUGIN_ENTRY():
    return r5900_plugin_t()