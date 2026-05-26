#include <tc48/cpu/pred.h>
#include <tc48/cpu.h>

#include <tc48/cpu/reg.h>
#include <tc48/word.h>

bool tc48_cpu_check_pred(tc48_cpu* cpu, tc48_triplet pred) {
    tc48_word cf = cpu->regs.data[TC48_CPU_REG_CF];
    tc48_trit_state s = tc48_word_get_trit(cf, TC48_CF_TRIT_S);
    tc48_trit_state v = tc48_word_get_trit(cf, TC48_CF_TRIT_V);
    tc48_trit_state c = tc48_word_get_trit(cf, TC48_CF_TRIT_C);

    switch (pred) {
    case TC48_PRED_AW: return true;

    case TC48_PRED_EQ: return s == TC48_CF_S_ZERO;
    case TC48_PRED_NE: return s != TC48_CF_S_ZERO;
    case TC48_PRED_LT: return s == TC48_CF_S_NEG;
    case TC48_PRED_GT: return s == TC48_CF_S_POS;
    case TC48_PRED_LE: return s == TC48_CF_S_ZERO || s == TC48_CF_S_NEG;
    case TC48_PRED_GE: return s == TC48_CF_S_ZERO || s == TC48_CF_S_POS;
    case TC48_PRED_ZR: return s == TC48_CF_S_ZERO;
    case TC48_PRED_NZ: return s != TC48_CF_S_ZERO;

    case TC48_PRED_CS: return c == TC48_CF_C_CARRY;
    case TC48_PRED_CC: return c != TC48_CF_C_CARRY;
    case TC48_PRED_BS: return c == TC48_CF_C_BORROW;
    case TC48_PRED_BC: return c != TC48_CF_C_BORROW;

    case TC48_PRED_VS: return v != TC48_CF_V_NONE;
    case TC48_PRED_VP: return v == TC48_CF_V_POS;
    case TC48_PRED_VN: return v == TC48_CF_V_NEG;
    case TC48_PRED_VC: return v == TC48_CF_V_NONE;
    }

    return true;
}
