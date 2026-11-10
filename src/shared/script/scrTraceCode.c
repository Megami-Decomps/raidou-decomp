#include "common.h"

/* Declarations from smt3-decomp src/smt3/script/scrTraceCode.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "newdata.h"

#include "scr.h"

enum code_operation
{
    CODE_OPERATION_ADD = 0,
    CODE_OPERATION_SUB = 1,
    CODE_OPERATION_MUL = 2,
    CODE_OPERATION_DIV = 3,
    CODE_OPERATION_OR = 4,
    CODE_OPERATION_AND = 5,
    CODE_OPERATION_EQ = 6,
    CODE_OPERATION_NEQ = 7,
    CODE_OPERATION_S = 8,
    CODE_OPERATION_L = 9,
    CODE_OPERATION_SE = 10,
    CODE_OPERATION_LE = 11,
    CODE_OPERATION_MAX = 12
};

extern scrProcessWork_s *nowpwork;

extern const scrCommandTable_t scrCommandTable[];

extern s32 (*codefunc[])(scrProcessWork_s *pw);

extern s32 PopInt(scrProcessWork_s *pwork);

extern f32 PopFloat(scrProcessWork_s *pwork);

extern void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type);

void PushInt(scrProcessWork_s *pwork, s32 data);

void PushFloat(scrProcessWork_s *pw, f32 value);

void PushStr(scrProcessWork_s *pwork, u8 *data);

void PushRet(scrProcessWork_s *pwork, s32 cp);

extern s32 itfMesMngMessage();

extern s32 itfMesMngOpenWindow();

extern s32 itfMesMngCloseWindow();

extern s32 itfMesMngSelect();

extern s32 itfMesMngOpenSelectWindow();

extern s32 itfMesMngCloseSelectWindow();

extern s32 itfMesMngMaskSelect();

extern s32 scrCommand_BIT_CHK();

extern s32 scrCommand_BIT_ON();

extern s32 scrCommand_BIT_OFF();

extern s32 scrCommand_RND();

extern s32 dds3_2263B8();

extern s32 snd_226390();

extern s32 scrCommand_SYNC();

extern s32 scrCommand_WAIT();

extern s32 scrCommand_FADE_IN();

extern s32 scrCommand_FADE_OUT();

extern s32 scrCommand_SEL_JMP();

extern s32 evtCommand_CAM_SEL();

extern s32 evtCommand_CAM_PATH_MOVE();

extern s32 evtCommand_SET_SKY();

extern s32 evtCommand_MODEL_DSP();

extern s32 evt_1E54B8();

extern s32 evtCommand_MODEL_MOVE();

extern s32 evt_1E50C0();

extern s32 evtCommand_CAM_SEL_FOVY();

extern s32 scrCommand_PAD_ACT();

extern s32 evtCommand_MODEL_SCL_XYZ();

extern s32 prm_MAKA_CHECK();

extern s32 prm_STOCK_FULL_CHECK();

extern s32 prm_PLAYER_ADD_PARAM();

extern s32 prm_DH_EQUIP();

extern s32 evtCommand_PMV_LOAD();

extern s32 evtCommand_PMV_RUN2();

extern s32 evtCommand_FIELD_CLS_STOP();

extern s32 scrCommand_PUT();

extern s32 evtCommand_GET_MOON();

extern s32 evtCommand_UNIT_CLS();

extern s32 evtCommand_GET_MOON16();

extern s32 evtCommand_RESTORE_BATTLE();

extern s32 evtCommand_UNIT_CLS_STOP();

extern s32 evtCommand_TEX_LOAD();

extern s32 evtCommand_TEX_ON();

extern s32 evtCommand_TEX_OFF();

extern s32 evtCommand_TEX_CLS();

extern s32 evtCommand_EXIT_EVENT();

extern s32 nbCommand_AI_TBL();

extern s32 nbCommand_AI_ACT_ATTACK();

extern s32 nbCommand_AI_ACT_ESCAPE();

extern s32 nbCommand_AI_ACT_WAIT();

extern s32 nbCommand_AI_ACT_SKILL();

extern s32 nbCommand_AI_TAR_AI();

extern s32 nbCommand_AI_TAR_LIFEMIN();

extern s32 nbCommand_AI_TAR_BAD();

extern s32 nbCommand_AI_TAR_ID();

extern s32 nbCommand_BTL_EVENT();

extern s32 nbCommand_AI_TAR_LIGHT();

extern s32 nbCommand_AI_TAR_DARK();

extern s32 nbCommand_AI_TAR_PLAYER();

extern s32 nbCommand_AI_TAR_RND();

extern s32 nbCommand_AI();

extern s32 nbCommand_BTL_PNL_OFF();

extern s32 nbCommand_BTL_PNL_ON();

extern s32 itfMesMng_SEL_SELNO();

extern s32 scrCommand_BLUR2_ON();

extern s32 scrCommand_BLUR2_OFF();

extern s32 scrCommand_BLUR_ALLOFF();

extern s32 evtCommand_MODEL_EXWAIT();

extern s32 dds3_2263E8();

extern s32 evtCommand_RTRN_TITLE();

extern s32 scrCommand_SOFTEN_BIAS();

extern s32 evtCommand_SET_SKY_A();

extern s32 evtCommand_MODEL_WAIT();

extern s32 evtCommand_MODEL_POS();

extern s32 evtCommand_MODEL_WAIT_MOT();

extern s32 evtCommand_MODEL_MOVE_MOT();

extern s32 evtCommand_FIELD_DSP();

extern s32 evtCommand_FIELD_CLS();

extern s32 itfCamp();

extern s32 dds3_22B880();

extern s32 dds3_22B8A8();

extern s32 movie_22B8C8();

extern s32 scrCommand_PAD_TRIG();

extern s32 scrCommand_PAD_PRESS();

extern s32 evtCommand_CAM_PATH_WAIT();

extern s32 evtCommand_MODEL_ROT();

extern s32 evtCommand_CALL_PUZZLEBOY();

extern s32 evtCommand_CALL_COMBINE();

extern s32 evtCommand_CALL_SAVE();

extern s32 evtCommand_CALL_RECOVER();

extern s32 evtCommand_CALL_SHOP();

extern s32 evtCommand_CALL_RAG();

extern s32 evtCommand_CALL_FIELD();

extern s32 fldCommand_ACT_WIN_DSP();

extern s32 fldCommand_DIR_HIT_CNTR();

extern s32 fldCommand_PAD_DISABLE();

extern s32 fldCommand_PAD_ENABLE();

extern s32 msel_2286A0();

extern s32 evtCommand_MOON_START();

extern s32 evtCommand_MOON_STOP();

extern s32 evtCommand_CHG_MOON();

extern s32 evtCommand_CALL_EVENT();

extern s32 evtCommand_CALL_BATTLE();

extern s32 evtCommand_MODEL_PLAYER();

extern s32 evtCommand_MODEL_ON();

extern s32 evtCommand_MODEL_OFF();

extern s32 evtCommand_OBJ_PATH_MOVE();

extern s32 evtCommand_OBJ_PATH_WAIT();

extern s32 scrCommand_SET_FOVY();

extern s32 fldCommand_ENCOUNT_ON();

extern s32 fldCommand_ENCOUNT_OFF();

extern s32 prm_ITEM_PLUS();

extern s32 itfMesMngSetVarScr();

extern s32 scrCommand_PUTS();

extern s32 evt_1E5158();

extern s32 evtCommand_MODEL_WAIT_HOKAN();

extern s32 evtCommand_CAM_CREATE();

extern s32 evtCommand_CAM_CLS();

extern s32 evtCommand_CAM_POS_XYZ();

extern s32 evtCommand_CAM_ROT_XYZ();

extern s32 evtCommand_MODEL_POS_XYZ();

extern s32 evtCommand_MODEL_ROT_XYZ();

extern s32 nbCommand_AI_CHK_MYHP();

extern s32 nbCommand_AI_CHK_FRHP();

extern s32 nbCommand_AI_CHK_DMGCNT();

extern s32 nbCommand_AI_CHK_FRCNT();

extern s32 nbCommand_AI_CHK_ENCNT();

extern s32 nbCommand_AI_CHK_MYBAD();

extern s32 nbCommand_AI_CHK_FRBAD();

extern s32 nbCommand_AI_CHK_ENBAD();

extern s32 nbCommand_AI_CHK_ENALLBAD();

extern s32 nbCommand_AI_CHK_ENID();

extern s32 nbCommand_AI_CHK_FRID();

extern s32 nbCommand_AI_CHK_ENHOJO();

extern s32 nbCommand_AI_CHK_FRHOJO();

extern s32 nbCommand_AI_CHK_BA();

extern s32 nbCommand_AI_CHK_ENLIGHT();

extern s32 nbCommand_AI_CHK_ENDARK();

extern s32 evtCommand_MODEL_MOVE2();

extern s32 nbCommand_AI_CHK_POW();

extern s32 evtCommand_LOAD_SOUND_E();

extern s32 evtCommand_BGM_PLAY_E();

extern s32 evtCommand_SE_PLAY_E();

extern s32 evtCommand_FADEOUT_E();

extern s32 evtCommand_BGM_VOL_DOWN();

extern s32 evtCommand_BGM_VOL_UP();

extern s32 itfMesMngIgnoreConfig();

extern s32 evtCommand_SEARCH_ID();

extern s32 evt_1E51F8();

extern s32 evtCommand_MODEL_WAIT_EXMOT();

extern s32 evtCommand_CALL_NEXT();

extern s32 evtCommand_CALL_FIELD2();

extern s32 scrCommand_FILTER_ALLOFF();

extern s32 evtCommand_EFFECT_LOAD();

extern s32 evtCommand_EFFECT_CLS();

extern s32 evtCommand_EFFECT_POS_XYZ();

extern s32 evtCommand_EFFECT_POS();

extern s32 evtCommand_EFFECT_ON();

extern s32 evtCommand_EFFECT_OFF();

extern s32 evtCommand_EFFECT_LINK();

extern s32 evtCommand_EFFECT_LINK2();

extern s32 evtCommand_EFFECT_UNLINK();

extern s32 fldCommand_SET_FIX_CAMERA();

extern s32 fldCommand_SET_CHASE_CAMERA();

extern s32 scrCommand_SCR_RUN();

extern s32 scrCommand_SCR_KILL();

extern s32 scrCommand_SCR_WAIT();

extern s32 scrCommand_SCR_STAT();

extern s32 scrCommand_SCR_TIME();

extern s32 evtCommand_PMV_RUN();

extern s32 scrCommand_QUAKE_ON();

extern s32 scrCommand_QUAKE_OFF();

extern s32 evtCommand_MODEL_LIGHT_OFF();

extern s32 evtCommand_MODEL_LIGHT_ON();

extern s32 evtCommand_MODEL_LIGHT_RGB();

extern s32 evtCommand_MODEL_LIGHT_DIR();

extern s32 evtCommand_MODEL_LIGHT_WAIT();

extern s32 evtCommand_MODEL_RGB();

extern s32 evtCommand_MODEL_A();

extern s32 scrCommand_SCENE_LIGHT_RGB();

extern s32 scrCommand_SCENE_LIGHT_DIR();

extern s32 scrCommand_SCENE_AMBIENT();

extern s32 evtCommand_PATH_STOP();

extern s32 evtCommand_PATH_START();

extern s32 evtCommand_PATH_RATE();

extern s32 evtCommand_PATH_DIR();

extern s32 scrCommand_SCENE_FOG_RGB();

extern s32 scrCommand_SCENE_FOG_PARA();

extern s32 scrCommand_BLUR();

extern s32 fldCommand_SET_CASH_F();

extern s32 fldCommand_FREE_CASH_F();

extern s32 func_00158548();

extern s32 fldCommand_CAM_DIR();

extern s32 fldCommand_CAM_SEL_PHOKAN();

extern s32 fldCommand_SET_ALLPANEL();

extern s32 evtCommand_MODEL_LIGHT_LINK();

extern s32 evtCommand_MODEL_LIGHT_UNLINK();

extern s32 evtCommand_LIGHT_PATH_MOVE();

extern s32 evtCommand_LIGHT_PATH_WAIT();

extern s32 evtCommand_LOAD_BE();

extern s32 evtCommand_MODEL_BE();

extern s32 evtCommand_FIELD_BE();

extern s32 evtCommand_EFFECT_BE();

extern s32 evtCommand_TEX_BE();

extern s32 evtCommand_EFFBED_LOAD();

extern s32 evtCommand_EFFBED_START();

extern s32 evtCommand_EFFECT_ROT_XYZ();

extern s32 evtCommand_EFFBED_BE();

extern s32 evtCommand_EFFECT_PATH_MOVE();

extern s32 evtCommand_EFFECT_PATH_WAIT();

extern s32 evtCommand_EFFMG1_LOAD();

extern s32 evtCommand_EFFMG1_BE();

extern s32 evtCommand_EFFMG2_LOAD();

extern s32 evtCommand_EFFMG2_BE();

extern s32 evtCommand_EFFMG1_POS();

extern s32 evtCommand_EFFMG2_POS();

extern s32 evt_1E9168();

extern s32 evtCommand_CAM_ROT_Q();

extern s32 evtCommand_FIELD_ANIME_START();

extern s32 evtCommand_FIELD_ANIME_STOP();

extern s32 evtCommand_MODEL_SHADOW_ON2();

extern s32 nbCommand_AI_CHK_PLV();

extern s32 nbCommand_AI_CHK_PPARTYLV();

extern s32 nbCommand_AI_GET_TURN();

extern s32 prm_DV_PLUS();

extern s32 nbCommand_AI_ACT_SUMMON();

extern s32 evtCommand_EFFMG2_PRESET();

extern s32 nbCommand_BTL_TAIKYO();

extern s32 nbCommand_BTL_ALLTAIKYO();

extern s32 nbCommand_BTL_PRESS();

extern s32 nbCommand_BTL_NAKAMA();

extern s32 evtCommand_PATH_LOOP_ON();

extern s32 evtCommand_PATH_LOOP_OFF();

extern s32 scrCommand_MUL_BLUR_PARA();

extern s32 scrCommand_MUL_BLUR_RGBA();

extern s32 scrCommand_MUL_BLUR_AREA();

extern s32 scrCommand_MUL_BLUR_ON();

extern s32 scrCommand_MUL_BLUR_OFF();

extern s32 scrCommand_DIST_BLUR_PARA();

extern s32 scrCommand_DIST_BLUR_RGBA();

extern s32 scrCommand_DIST_BLUR_AREA();

extern s32 scrCommand_DIST_BLUR_ON();

extern s32 scrCommand_DIST_BLUR_OFF();

extern s32 nbCommand_BTL_CAM_SET();

extern s32 nbCommand_BTL_CAM_ORG();

extern s32 nbCommand_AI_SET_CRITICAL();

extern s32 evtCommand_READ_BE();

extern s32 evtCommand_CHK_BE();

extern s32 evtCommand_DEL_BE();

extern s32 nbCommand_BTL_CAM_1SHOT();

extern s32 nbCommand_BTL_CAM_2SHOT();

extern s32 nbCommand_BTL_CAM_BOUGAI();

extern s32 fldCommand_FLD_CHK_BTL();

extern s32 fldCommand_FLD_CHK_ANA();

extern s32 fldCommand_FLD_CHK_SISETU();

extern s32 evtCommand_CALL_FIELD3();

extern s32 fldCommand_FLD_KANKYO();

extern s32 fldCommand_FLD_FILTER();

extern s32 fldCommand_FLD_MODEL_ON();

extern s32 fldCommand_FLD_MODEL_OFF();

extern s32 fldCommand_FLD_ANIME_ON();

extern s32 fldCommand_FLD_ANIME_OFF();

extern s32 fldCommand_FLD_ATARI_ON();

extern s32 fldCommand_FLD_ATARI_OFF();

extern s32 fldCommand_FLD_MODEL_ON2();

extern s32 fldCommand_FLD_MODEL_OFF2();

extern s32 fldCommand_FLD_ANIME_ON2();

extern s32 fldCommand_FLD_ANIME_OFF2();

extern s32 fldCommand_FLD_ANIME_ID2();

extern s32 fldCommand_FLD_NPC_ON();

extern s32 fldCommand_FLD_NPC_OFF();

extern s32 fldCommand_FLD_GIMIC_HOJI();

extern s32 fldCommand_FLD_MAP_ON();

extern s32 fldCommand_FLD_MAP_OFF();

extern s32 fldCommand_SET_CAMTBL();

extern s32 fldCommand_FLD_GET_TAKARA_TBL();

extern s32 fldCommand_FLD_SET_TAKARA_OPEN();

extern s32 fldCommand_FLD_CHK_TAKARA_OPEN();

extern s32 itfMesMngSetMesPositionScript();

extern s32 itfMesMngSetMesPosDefault();

extern s32 itfMesMngValidateNext();

extern s32 trm_210A00();

extern s32 trm_210AF8();

extern s32 func_00210B38();

extern s32 fcl_214428();

extern s32 fcl_214458();

extern s32 func_00210B68();

extern s32 func_00210B90();

extern s32 nme_298698();

extern s32 prm_DH_PLUS();

extern s32 evtCommand_MODEL_SHADOW_ON();

extern s32 evtCommand_MODEL_SHADOW_OFF();

extern s32 evtCommand_BOKASI_ON();

extern s32 evtCommand_BOKASI_OFF();

extern s32 scrCommand_MUL_BLUR_SWAP();

extern s32 scrCommand_DIST_BLUR_SWAP();

extern s32 scrCommand_BLUR2_PARA();

extern s32 scrCommand_BLUR2_RGBA();

extern s32 scrCommand_BLUR2_AREA();

extern s32 scrCommand_BLUR2_SWAP();

extern s32 scrCommand_FILTER2_PARA();

extern s32 scrCommand_FILTER2_RGBA();

extern s32 scrCommand_FILTER2_SWAP();

extern s32 scrCommand_FILTER2_ON();

extern s32 scrCommand_FILTER2_OFF();

extern s32 scrCommand_MFILTER_PARA();

extern s32 scrCommand_MFILTER_RGBA();

extern s32 scrCommand_MFILTER_AREA();

extern s32 scrCommand_MFILTER_SWAP();

extern s32 scrCommand_MFILTER_ON();

extern s32 scrCommand_MFILTER_OFF();

extern s32 evtCommand_BGM_TRANS_E();

extern s32 prm_MAKA_PLUS();

extern s32 prm_PARTY_DAMAGE();

extern s32 itfMesMngWindowType();

extern s32 itfMesMngSelectDefKey();

extern s32 evt_1E91A8();

extern s32 evt_1E91E8();

extern s32 evt_1E9220();

extern s32 evt_1E9248();

extern s32 evt_1E9270();

extern s32 scrCommand_RIP_BLUR_ON();

extern s32 scrCommand_RIP_BLUR_OFF();

extern s32 scrCommand_RIP_BLUR_PARA();

extern s32 scrCommand_RIP_BLUR_RGBA();

extern s32 scrCommand_RIP_BLUR_AREA();

extern s32 scrCommand_RIP_BLUR_SWAP();

extern s32 evt_1E9298();

extern s32 evt_1E92D8();

extern s32 evtCommand_EFFECT_PRESET();

extern s32 nbCommand_AI_CHK_MYMP();

extern s32 nbCommand_AI_GET_PRESSKOUDO();

extern s32 nbCommand_AI_GET_ALLKOUDO();

extern s32 nbCommand_AI_GET_MYLV();

extern s32 prm_UDV_LEAVE();

extern s32 prm_UDV_CHECK();

extern s32 prm_UDV_DELETE_OK();

extern s32 prm_UDV_DELETE_NG();

extern s32 nbCommand_BTL_FORM_ALL_ON();

extern s32 nbCommand_BTL_FORM_PLY_ON();

extern s32 nbCommand_BTL_FORM_ENE_ON();

extern s32 nbCommand_AI_GET_MYAISYO();

extern s32 evtCommand_CALL_STAFF();

extern s32 prm_STOCK_PLUS();

extern s32 nbCommand_AI_TAR_BAARLV();

extern s32 prm_UDV_GETID();

extern s32 nbCommand_AI_TAR_LVMIN();

extern s32 prm_PLAYER_GET_PARAM();

extern s32 nbCommand_AI_GET_FUREA();

extern s32 evtCommand_CALL_ENDSAVE();

extern s32 evtCommand_KUTI();

extern s32 itfMesMngWindowTypeAlpha();

extern s32 fldCommand_FLD_ANAHORI_INIT();

extern s32 fldCommand_FLD_ANAHORI_GET();

extern s32 fldCommand_FLD_ANAHORI_NEXT();

extern s32 fldCommand_FLD_ANAHORI_DEL();

extern s32 evtCommand_KUTISET();

extern s32 evtCommand_CALL_BATTLE2();

extern s32 evtCommand_EFFBED_BE_EVO();

extern s32 evtCommand_EFFBED_START_EVO();

extern s32 nbCommand_AI_CHK_ESCAPE();

extern s32 evt_1E92F8();

extern s32 movie_22B8F0();

extern s32 prm_BOSS_PRESS_REC();

extern s32 prm_NAKAMA_GET_PARAM();

extern s32 prm_PLAYER_GET_LND();

extern s32 prm_NAKAMA_DEAD();

extern s32 prm_NAKAMA_CHK_PIKUKO();

extern s32 prm_PIKUKO_GETID();

extern s32 prm_PARTY_KAIFUKU();

extern s32 prm_DV_COPY_PARAM();

extern s32 nbCommand_BTL_AI_ROT_CLR();

extern s32 nbCommand_BTL_AI_ROT_NEXT();

extern s32 nbCommand_BTL_AI_ROT_GET();

extern s32 nbCommand_AI_CHK_ENHOJOMAX();

extern s32 nbCommand_AI_CHK_FRHOJOMAX();

extern s32 nbCommand_BTL_AI_CHANGE_OFF();

extern s32 nbCommand_BTL_AI_CHANGE_ON();

extern s32 nbCommand_AI_CHK_ENHAMAMHK();

extern s32 nbCommand_AI_TAR_JOKEN();

extern s32 nbCommand_AI_CHK_SKILLNOMP();

extern s32 prm_PLAYER_GET_LEVEL();

extern s32 prm_MAKA_PLUS_PER();

extern s32 prm_DV_CHECK();

extern s32 evtCommand_PMV_LOAD_END_WAIT();

extern s32 prm_PARTY_KAIFUKU2();

extern s32 evtCommand_TEX_SWAP();

extern s32 evtCommand_TEX_PRI();

extern s32 evtCommand_TEX_SIZE();

extern s32 evtCommand_TEX_POS();

extern s32 evtCommand_TEX_ALP();

extern s32 evtCommand_TEX_ZOOM();

extern s32 evtCommand_TEX_OT();

extern s32 evtCommand_TEX_STAT();

extern s32 fldCommand_FLD_TITLE_START2();

extern s32 fldCommand_FLD_GET_KIRETA();

extern s32 fldCommand_FLD_GET_ITEMBOX_RND();

extern s32 fldCommand_FLD_MSE_PLAY2();

extern s32 fldCommand_FLD_MSE_STOP2();

extern s32 fldCommand_SET_NPC_CAMERA();

extern s32 fldCommand_FLD_MODEL_DANTE();

extern s32 fldCommand_FLD_DANTE_AI_STOP();

extern s32 fldCommand_FLD_DANTE_AI_START();

extern s32 fldCommand_FLD_DANTE_STANCE();

extern s32 fldCommand_FLD_BGM_RELOAD();

extern s32 fldCommand_FLD_BGM_RELOAD_WAIT();

extern s32 fldCommand_FLD_WARP_ELV_GETTBL();

extern s32 fldCommand_FLD_WARP_ELV_EXIT();

extern s32 fldCommand_FLD_WARP_CALL_FIELD();

extern s32 fldCommand_FLD_WARP_CASH_F();

extern s32 fldCommand_FLD_WARP_RUN_START();

extern s32 fldCommand_FLD_INF_NEXT();

extern s32 fldCommand_FLD_INF_GET();

extern s32 fldCommand_FLD_INF_GETPANEL();

extern s32 fldCommand_FLD_WARP_HASIGO_GETTBL();

extern s32 fldCommand_FLD_ELV_FLOORSTART();

extern s32 fldCommand_FLD_ELV_FLOOREND();

extern s32 fldCommand_FLD_ELV_ENDCHK();

extern s32 fldCommand_FLD_WARP_GET_DOOR();

extern s32 fldCommand_FLD_EXIT_CAMERA();

extern s32 fldCommand_FLD_INF_GOWAP();

extern s32 fldCommand_FLD_CALL_WAPEX();

extern s32 fldCommand_FLD_BGM_PLAY();

extern s32 fldCommand_FLD_BGM_CONTINUE();

extern s32 fldCommand_FLD_BGM_PAUSE();

extern s32 fldCommand_FLD_BGM_STOP();

extern s32 fldCommand_FLD_SE_PLAY();

extern s32 fldCommand_FLD_SE_STOP();

extern s32 fldCommand_FLD_BGM_FADE();

extern s32 fldCommand_FLD_MSE_PLAY();

extern s32 fldCommand_FLD_NPC_SINEN();

extern s32 fldCommand_FLD_WARP_GET_WARPEFF();

extern s32 fldCommand_FLD_TITLE_START();

extern s32 fldCommand_ACT_WIN_DSP2();

extern s32 fldCommand_FLD_SET_ENCOUNT();

extern s32 fldCommand_ACT_WIN_DSP3();

extern s32 fldCommand_FLD_MODEL_LIGHT();

extern s32 fldCommand_FLD_SAGASI_AKUMA();

extern const scrCommandTable_t scrCommandTable[544];

s32 PopInt(scrProcessWork_s *pwork);

f32 PopFloat(scrProcessWork_s *pwork);

s32 CodeFunc_PushI(scrProcessWork_s *pwork);

s32 CodeFunc_PushIS(scrProcessWork_s *pwork);

s32 CodeFunc_PushF(scrProcessWork_s *pwork);

s32 CodeFunc_PushIX(scrProcessWork_s *pwork);

s32 CodeFunc_PushFX(scrProcessWork_s *pwork);

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork);

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork);

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork);

s32 CodeFunc_PushReg(scrProcessWork_s *pwork);

s32 CodeFunc_PopIX(scrProcessWork_s *pwork);

s32 CodeFunc_PopFX(scrProcessWork_s *pwork);

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork);

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork);

s32 CodeFunc_Proc(scrProcessWork_s *pwork);

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork);

s32 CodeFunc_End(scrProcessWork_s *pwork);

s32 CodeFunc_Jump(scrProcessWork_s *pwork);

s32 CodeFunc_Call(scrProcessWork_s *pwork);

s32 CodeFunc_Run(scrProcessWork_s *pwork);

s32 CodeFunc_Goto(scrProcessWork_s *pwork);

extern s32 CodeFunc_2Operation_i1;

extern s32 CodeFunc_2Operation_i2;

extern s32 CodeFunc_2Operation_ri;

extern f32 CodeFunc_2Operation_f1;

extern f32 CodeFunc_2Operation_f2;

extern f32 CodeFunc_2Operation_rf;

extern s32 CodeFunc_2Operation_t1;

extern s32 CodeFunc_2Operation_t2;

void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type);

s32 CodeFunc_Add(scrProcessWork_s *pwork);

s32 CodeFunc_Sub(scrProcessWork_s *pwork);

s32 CodeFunc_Mul(scrProcessWork_s *pwork);

s32 CodeFunc_Div(scrProcessWork_s *pwork);

s32 CodeFunc_Minus(scrProcessWork_s *pwork);

s32 CodeFunc_Not(scrProcessWork_s *pwork);

s32 CodeFunc_Or(scrProcessWork_s *pwork);

s32 CodeFunc_And(scrProcessWork_s *pwork);

s32 CodeFunc_EQ(scrProcessWork_s *pwork);

s32 CodeFunc_NEQ(scrProcessWork_s *pwork);

s32 CodeFunc_S(scrProcessWork_s *pwork);

s32 CodeFunc_L(scrProcessWork_s *pwork);

s32 CodeFunc_SE(scrProcessWork_s *pwork);

s32 CodeFunc_LE(scrProcessWork_s *pwork);

s32 CodeFunc_IF(scrProcessWork_s *pwork);

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork);

s32 scrGetIntPara(s32 n);

f32 scrGetFloatPara(s32 n);

u8 *scrGetStrPara(s32 n);

void scrSetIntReturnValue(s32 i);

void scrSetFloatReturnValue(f32 f);

s32 scrGetProcedureCodeAddress(s32 procindex);

s32 scrGetLabelCodeAddress(s32 labelindex);

s32 scrGetCodePointer(void);

void scrSetCodePointer(s32 cp);

s32 scrGetScriptTimer(void);

s32 scrGetCommandTimer(void);

s32 scrGetMessageHandle(void);

scrProcessWork_s *scrGetScriptHandle(void);

void *scrGetData(void);
/* End of declarations from src/smt3/script/scrTraceCode.c */

void PushInt(scrProcessWork_s *pwork, s32 data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_INT;
    pwork->StackData[pwork->SP].i = data;
    pwork->SP++;
}

void PushFloat(scrProcessWork_s *pw, f32 value)
{
    pw->StackType[pw->SP] = SCR_STACK_TYPE_FLOAT;
    pw->StackData[pw->SP].f = value;
    pw->SP++;
}

void PushStr(scrProcessWork_s *pwork, u8 *data)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_STRING;
    pwork->StackData[pwork->SP].i = EE_ADDR_S(data);
    pwork->SP++;
}

void PushRet(scrProcessWork_s *pwork, s32 cp)
{
    pwork->StackType[pwork->SP] = SCR_STACK_TYPE_RET;
    pwork->StackData[pwork->SP].i = cp;
    pwork->SP++;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", PopInt);

INCLUDE_ASM(const s32, "script/scrTraceCode", PopFloat);

s32 CodeFunc_PushI(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushInt(pwork, pwork->Code[pwork->CP].i);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushIS(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushF(scrProcessWork_s *pwork)
{
    pwork->CP++;
    PushFloat(pwork, pwork->Code[pwork->CP].f);
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PushIX);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PushFX);

s32 CodeFunc_PushLIX(scrProcessWork_s *pwork)
{
    PushInt(pwork, pwork->LocalInt[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushLFX(scrProcessWork_s *pwork)
{
    PushFloat(pwork, pwork->LocalFloat[pwork->Code[pwork->CP].s.s1]);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushSTR(scrProcessWork_s *pwork)
{
    PushStr(pwork, pwork->StringData + pwork->Code[pwork->CP].s.s1);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PushReg(scrProcessWork_s *pwork)
{
    pwork->StackType[pwork->SP] = pwork->StackType[27];
    pwork->StackData[pwork->SP].i = pwork->StackData[27].i;
    pwork->SP++;
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PopIX);

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_PopFX);

s32 CodeFunc_PopLIX(scrProcessWork_s *pwork)
{
    pwork->LocalInt[pwork->Code[pwork->CP].s.s1] = PopInt(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_PopLFX(scrProcessWork_s *pwork)
{
    pwork->LocalFloat[pwork->Code[pwork->CP].s.s1] = PopFloat(pwork);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Proc(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

/* A pending command yields without consuming its arguments. */
s32 CodeFunc_Comm(scrProcessWork_s *pwork)
{
    s32 commandCP = pwork->CP;
    s32 commandIndex = pwork->Code[commandCP].s.s1;

    nowpwork = pwork;
    if (scrCommandTable[commandIndex].comm() == 0) {
        return CODE_RET_SYNC;
    }
    pwork->SP -= scrCommandTable[commandIndex].paranum;
    if (commandCP == pwork->CP) {
        pwork->CP = commandCP + 1;
    }
    return CODE_RET_NEXT;
}

s32 CodeFunc_End(scrProcessWork_s *pwork)
{
    if (pwork->SP == 0) {
        return CODE_RET_END;
    }

    pwork->CP = PopInt(pwork) + 1;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Jump(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Call(scrProcessWork_s *pwork)
{
    PushRet(pwork, pwork->CP);
    pwork->CP = pwork->ProcTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Run(scrProcessWork_s *pwork)
{
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Goto(scrProcessWork_s *pwork)
{
    pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    return CODE_RET_NEXT;
}

void CodeFunc_2Operation(scrProcessWork_s *pwork, s32 type)
{
    CodeFunc_2Operation_t1 = (s8)pwork->StackType[pwork->SP - 1];
    CodeFunc_2Operation_t2 = (s8)pwork->StackType[pwork->SP - 2];
    if ((CodeFunc_2Operation_t1 == 0 || CodeFunc_2Operation_t1 == 2) && (CodeFunc_2Operation_t2 == 0 || CodeFunc_2Operation_t2 == 2)) {
        CodeFunc_2Operation_i1 = PopInt(pwork);
        CodeFunc_2Operation_i2 = PopInt(pwork);
        switch (type) {
        case 0:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 + CodeFunc_2Operation_i2;
            break;
        case 1:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 - CodeFunc_2Operation_i2;
            break;
        case 2:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 * CodeFunc_2Operation_i2;
            break;
        case 3:
            CodeFunc_2Operation_ri = CodeFunc_2Operation_i1 / CodeFunc_2Operation_i2;
            break;
        case 4:
            if (CodeFunc_2Operation_i1 != 0 || CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 5:
            if (CodeFunc_2Operation_i1 != 0 && CodeFunc_2Operation_i2 != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 6:
            if (CodeFunc_2Operation_i1 == CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 7:
            if (CodeFunc_2Operation_i1 != CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 8:
            if (CodeFunc_2Operation_i1 < CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 9:
            if (CodeFunc_2Operation_i1 > CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 10:
            if (CodeFunc_2Operation_i1 <= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        case 11:
            if (CodeFunc_2Operation_i1 >= CodeFunc_2Operation_i2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            break;
        }
        PushInt(pwork, CodeFunc_2Operation_ri);
    } else {
        CodeFunc_2Operation_f1 = PopFloat(pwork);
        CodeFunc_2Operation_f2 = PopFloat(pwork);
        switch (type) {
        case 0:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 + CodeFunc_2Operation_f2;
            break;
        case 1:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 - CodeFunc_2Operation_f2;
            break;
        case 2:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 * CodeFunc_2Operation_f2;
            break;
        case 3:
            CodeFunc_2Operation_rf = CodeFunc_2Operation_f1 / CodeFunc_2Operation_f2;
            break;
        case 4:
            if (dpcmp(fptodp(CodeFunc_2Operation_f1), 0.0) != 0 ||
                dpcmp(fptodp(CodeFunc_2Operation_f2), 0.0) != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 5:
            if (dpcmp(fptodp(CodeFunc_2Operation_f1), 0.0) != 0 &&
                dpcmp(fptodp(CodeFunc_2Operation_f2), 0.0) != 0) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 6:
            if (CodeFunc_2Operation_f1 == CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 7:
            if (CodeFunc_2Operation_f1 != CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 8:
            if (CodeFunc_2Operation_f1 < CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 9:
            if (CodeFunc_2Operation_f1 > CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 10:
            if (CodeFunc_2Operation_f1 <= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        case 11:
            if (CodeFunc_2Operation_f1 >= CodeFunc_2Operation_f2) {
                CodeFunc_2Operation_ri = 1;
            } else {
                CodeFunc_2Operation_ri = 0;
            }
            PushInt(pwork, CodeFunc_2Operation_ri);
            return;
        }
        PushFloat(pwork, CodeFunc_2Operation_rf);
    }
}

s32 CodeFunc_Add(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_ADD);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Sub(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SUB);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Mul(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_MUL);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Div(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_DIV);
    pwork->CP++;
    return CODE_RET_NEXT;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", CodeFunc_Minus);

s32 CodeFunc_Not(scrProcessWork_s *pwork)
{
    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        PushInt(pwork, PopInt(pwork) == 0);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        PushInt(pwork, PopFloat(pwork) == 0.0f);
        break;
    case SCR_STACK_TYPE_RET:
        break;
    }

    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_Or(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_OR);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_And(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_AND);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_EQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_EQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_NEQ(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_NEQ);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_S(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_S);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_L(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_L);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_SE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_SE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_LE(scrProcessWork_s *pwork)
{
    CodeFunc_2Operation(pwork, CODE_OPERATION_LE);
    pwork->CP++;
    return CODE_RET_NEXT;
}

s32 CodeFunc_IF(scrProcessWork_s *pwork)
{
    s32 cond;

    switch ((s8)pwork->StackType[pwork->SP - 1]) {
    case SCR_STACK_TYPE_INT:
    case SCR_STACK_TYPE_INTVAR:
        cond = PopInt(pwork);
        break;
    case SCR_STACK_TYPE_FLOAT:
    case SCR_STACK_TYPE_FLOATVAR:
        cond = PopFloat(pwork) != 0.0f;
        break;
    case SCR_STACK_TYPE_RET:
    default:
        cond = 0;
        break;
    }

    if (cond) {
        pwork->CP++;
    } else {
        pwork->CP = pwork->LabelTable[pwork->Code[pwork->CP].s.s1].CodeAddress;
    }
    return CODE_RET_NEXT;
}

/* Continue until an instruction completes the procedure or yields a tick. */
s32 scrTraceCode(scrProcessWork_s *pwork)
{
    s32 result;

    while (1) {
        result = codefunc[pwork->Code[pwork->CP].s.s0](pwork);
        if (result == CODE_RET_END) {
            pwork->CommandTimer = 0;
            return SCR_RESULT_END;
        }
        if (result == CODE_RET_SYNC) {
            break;
        }
        if (result == CODE_RET_NEXT) {
            pwork->CommandTimer = 0;
        }
    }
    pwork->CommandTimer++;
    pwork->ScriptTimer++;
    return SCR_RESULT_CONTINUE;
}

INCLUDE_ASM(const s32, "script/scrTraceCode", scrGetIntPara);

INCLUDE_ASM(const s32, "script/scrTraceCode", scrGetFloatPara);

u8 *scrGetStrPara(s32 n)
{
    scrProcessWork_s *pw = nowpwork;
    s32 stackIndex = pw->SP - n - 1;
    s32 stackType = (s8)pw->StackType[stackIndex];

    if (stackType >= 0) {
        if (stackType >= SCR_STACK_TYPE_STRING) {
            if (stackType == SCR_STACK_TYPE_STRING) {
                return EE_ADDR_AS(u8 *, pw->StackData[stackIndex].i);
            }
        }
    }
    return NULL;
}

void scrSetIntReturnValue(s32 i)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_INT;
    nowpwork->StackData[27].i = i;
}

void scrSetFloatReturnValue(f32 f)
{
    nowpwork->StackType[27] = SCR_STACK_TYPE_FLOAT;
    nowpwork->StackData[27].f = f;
}

s32 scrGetProcedureCodeAddress(s32 procindex)
{
    return nowpwork->ProcTable[procindex].CodeAddress;
}

s32 scrGetLabelCodeAddress(s32 labelindex)
{
    return nowpwork->LabelTable[labelindex].CodeAddress;
}
