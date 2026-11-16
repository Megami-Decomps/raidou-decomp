# Hex-Rays pass for EE code: compare and branch on the low 32 bits of a register. ee-gcc keeps ints
# sign-extended in 64-bit GPRs, so bltz/bnez/slt on them are 32-bit tests; wrong for real u64 values.
import ida_hexrays

_opt = None
CMP = {getattr(ida_hexrays, n) for n in (
    "m_jz", "m_jnz", "m_jl", "m_jge", "m_jg", "m_jle", "m_jb", "m_jae", "m_ja", "m_jbe",
    "m_setz", "m_setnz", "m_setl", "m_setge", "m_setg", "m_setle", "m_setb", "m_setae", "m_seta", "m_setbe")}


def _narrow(op):
    if op.size != 8:
        return False
    if op.t == ida_hexrays.mop_r:
        op.size = 4
        return True
    if op.t == ida_hexrays.mop_n:
        v = op.nnn.value & 0xFFFFFFFFFFFFFFFF
        lo = v & 0xFFFFFFFF
        if (lo | (0xFFFFFFFF00000000 if lo & 0x80000000 else 0)) != v:
            return False
        op.make_number(lo, 4)
        return True
    return False


def _narrowable(op):
    if op.size != 8:
        return False
    if op.t == ida_hexrays.mop_r:
        return True
    if op.t == ida_hexrays.mop_n:
        v = op.nnn.value & 0xFFFFFFFFFFFFFFFF
        lo = v & 0xFFFFFFFF
        return (lo | (0xFFFFFFFF00000000 if lo & 0x80000000 else 0)) == v
    return False


class _visitor(ida_hexrays.minsn_visitor_t):
    def __init__(self):
        ida_hexrays.minsn_visitor_t.__init__(self)
        self.n = 0

    def visit_minsn(self):
        ins = self.curins
        if ins.opcode in CMP and _narrowable(ins.l) and _narrowable(ins.r):
            _narrow(ins.l)
            _narrow(ins.r)
            self.n += 1
        elif ins.opcode == ida_hexrays.m_jtbl and _narrowable(ins.l):
            _narrow(ins.l)
            self.n += 1
        return 0


class ee32_t(ida_hexrays.optinsn_t):
    def func(self, blk, ins, optflags):
        if blk.mba.maturity > ida_hexrays.MMAT_PREOPTIMIZED:
            return 0
        v = _visitor()
        ins.for_all_insns(v)
        return v.n


def _install():
    global _opt
    if _opt is None:
        _opt = ee32_t()
        _opt.install()
        print("[ee32] installed")


def _remove():
    global _opt
    if _opt is not None:
        _opt.remove()
        _opt = None
