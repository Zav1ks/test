/*******************************************************************************
*                                                                              *
*  Имя файла: ILCA.c                                                           *
*                                                                              *
*  Текущая версия файла: 003												   *
*                                                                              *
*  ИСТОРИЯ СОЗДАНИЯ:                                                           *
*  вер.:   дата:         программист:       причина изменений:                 *
*    000    23-05-2026   Миссарова А.И.     Создание модуля                    *
*    001    23-05-2026   Миссарова А.И.     Создание основного функционала     *
*    002    25-05-2026   Миссарова А.И.     Исправление логики получения       *
*												            значений амплитуд  *
*    003    25-05-2026   Миссарова А.И.     Разбиение на более мелкие функции  *
*												              для трассировки  *
*******************************************************************************/

#include "ILCA.h"

#include <math.h>
#include <stddef.h>
#include <time.h>

// пара соответствующих частот КРМ и ГРМ
struct ilca_frequency_pair
{
    float krm_mhz;
    float grm_mhz;
};

// пакет амплитуд, считываемый из адаптера приемника
struct ilca_amplitudes
{
    int32_t A_L_90;
    int32_t A_L_150;
    int32_t A_G_90;
    int32_t A_G_150;
};

static const float theta = 3.0; // угол наклона глиссады
static const float allowedDegError = 0.06; // допустимая погрешность вычисления отклонения
static const float DegToRad = 0.01745329251994329577; // коэффициент перевода градусов в радианы
static const float ampMin = 5.0; // минимально допустимая амплитуда
static const float allowedFreqError = 0.001; // допустимая погрешность вычисления частоты

static const struct ilca_frequency_pair k_frequency_table[40] =
{
    {108.10, 334.70},
    {108.15, 334.55},
    {108.30, 333.95},
    {108.35, 334.10},
    {108.50, 329.90},
    {108.55, 329.75},
    {108.70, 330.50},
    {108.75, 330.35},
    {108.90, 329.30},
    {108.95, 329.15},
    {109.10, 331.40},
    {109.15, 331.25},
    {109.30, 332.00},
    {109.35, 331.85},
    {109.50, 332.60},
    {109.55, 332.45},
    {109.70, 333.20},
    {109.75, 333.05},
    {109.90, 333.80},
    {109.95, 333.65},
    {110.10, 334.40},
    {110.15, 334.25},
    {110.30, 335.00},
    {110.35, 334.85},
    {110.50, 329.60},
    {110.55, 333.45},
    {110.70, 330.20},
    {110.75, 333.05},
    {110.90, 330.80},
    {110.95, 330.65},
    {111.10, 331.70},
    {111.15, 331.55},
    {111.30, 332.30},
    {111.35, 332.15},
    {111.50, 332.90},
    {111.55, 332.75},
    {111.70, 333.50},
    {111.75, 333.35},
    {111.90, 331.10},
    {111.95, 330.95}
};

// регистры адаптера, в которые записываются вычисленные значения
// актуальность данных регулируется последовательным номером пакета и временной меткой
volatile uint32_t ilca_hw_amp_seq = 0; // последовательный номер пакета амплитуд
volatile uint32_t ilca_hw_amp_timestamp_ms = 0; // временная метка прихода последнего пакета амплитуд
// значения амплитуд
volatile int32_t ilca_hw_amp_A_L_90 = 0;
volatile int32_t ilca_hw_amp_A_L_150 = 0;
volatile int32_t ilca_hw_amp_A_G_90 = 0;
volatile int32_t ilca_hw_amp_A_G_150 = 0;

static float last_krm = -1.0; // последняя принятая частота КРМ [МГц], -1.0 означает "еще не инициализировано"
static float last_grm; // последняя вычисленная частота ГРМ [МГц]
static uint32_t last_tune_time_ms = 0; // время последней перенастройки по новой КРМ [мс] по CLOCK_MONOTONIC
static uint32_t last_used_amp_seq = UINT32_MAX; // номер последнего использованного пакета амплитуд

static uint32_t now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)((uint64_t)ts.tv_sec * 1000 + (uint64_t)(ts.tv_nsec / 1000000));
}

// чтение пакета амплитуд
static int read_amplitudes_from_adapter(ilca_amplitudes *amps,
                                        uint32_t *seq,
                                        uint32_t *timestamp_ms)
{
	if (amps == NULL || seq == NULL || timestamp_ms == NULL)
        return 1;

	uint32_t seq_before = ilca_hw_amp_seq; // номер пакета до чтения амплитуд

	// чтение амплитуд и временной метки
    amps->A_L_90 = ilca_hw_amp_A_L_90;
    amps->A_L_150 = ilca_hw_amp_A_L_150;
    amps->A_G_90 = ilca_hw_amp_A_G_90;
    amps->A_G_150 = ilca_hw_amp_A_G_150;
    *timestamp_ms = ilca_hw_amp_timestamp_ms;

	uint32_t seq_after = ilca_hw_amp_seq; // номер пакета после чтения амплитуд

	// если в процессе чтения данные обновились - нарушена целостность пакета амплитуд
	if (seq_before != seq_after)
        return 1;

    *seq = seq_after;
    return 0;
}

// выбор частоты ГРМ по частоте КРМ
static int find_grm_mhz(float krm_mhz, float *grm_mhz)
{
    size_t i;

	if (!isfinite(krm_mhz))
        return 0;

	for (i = 0; i < 40; ++i)
		if (fabsf(krm_mhz - k_frequency_table[i].krm_mhz) <= allowedFreqError)
		{
            *grm_mhz = k_frequency_table[i].grm_mhz;
            return 1;
		}

    return 0;
}

// формирование выходных значений в случае какой-либо ошибки
static struct ilca_output make_fail(void)
{
    struct ilca_output out;

    out.ksi_k_deg = NAN;
    out.ksi_g_deg = NAN;
    out.fail = true;
    return out;
}

// проверка корректности базовых входных данных
static int validate_input(const struct ilca_input *in)
{
	if (in == NULL)
        return 0;

	if (!isfinite(in->krm_mhz))
        return 0;

    return 1;
}

// обновление частоты ГРМ при изменении частоты КРМ
static int update_grm(float krm_mhz, uint32_t start_ms)
{
	if (last_krm < 0.0 || fabsf(krm_mhz - last_krm) > allowedFreqError)
	{
		if (!find_grm_mhz(krm_mhz, &last_grm))
            return 0;

		if (last_krm >= 0.0)
            last_tune_time_ms = start_ms;

        last_krm = krm_mhz;
    }
    return 1;
}

// получение значений амплитуд (до 20 мс)
static int read_amps(struct ilca_amplitudes *amps, uint32_t start_ms)
{
    uint32_t amp_seq;
    uint32_t amp_timestamp_ms;

	while ((uint32_t)(now_ms() - start_ms) <= 20)
	{
		if (read_amplitudes_from_adapter(amps, &amp_seq, &amp_timestamp_ms) != 0)
            continue;

		if (amp_seq == last_used_amp_seq)
            continue;

		if (amp_timestamp_ms < last_tune_time_ms)
            continue;

		if (now_ms() - amp_timestamp_ms > 20)
			continue;

        last_used_amp_seq = amp_seq;
        return 1;
    }

    return 0;
}

// проверка корректности значений амплитуд
static int validate_amps(const struct ilca_amplitudes *amps)
{
    if (amps->A_L_90 < ampMin ||
        amps->A_L_150 < ampMin ||
        amps->A_G_90 < ampMin ||
		amps->A_G_150 < ampMin)
        return 0;

    return 1;
}
// выбор коэффициента калибровки по оставшемуся расстоянию до ВПП
static int calc_k(float distance_km, float *K)
{
	if (!isfinite(distance_km) || distance_km < 0.0)
		return 0;

	if (distance_km <= 5.0)
		 *K = 4.0;

	if (distance_km <= 10.0)
		*K =  2.5;

	if (distance_km <= 18.5)
		*K =  1.8;

	if (distance_km <= 32.0)
		*K =  1.2;

	*K =  1.0;

    return 1;
}

// вычисление DDM по курсу
static float calc_ddm_loc(const struct ilca_amplitudes *amps)
{
    return ((float)amps->A_L_150 - (float)amps->A_L_90) /
           ((float)amps->A_L_150 + (float)amps->A_L_90);
}

// вычисление DDM по глиссаде
static float calc_ddm_gs(const struct ilca_amplitudes *amps)
{
    return ((float)amps->A_G_150 - (float)amps->A_G_90) /
           ((float)amps->A_G_150 + (float)amps->A_G_90);
}

// вычисление отклонения по курсу
static float calc_ksi_k(float K, float ddm_loc)
{
    float x_deg;
    float cos_value;

    x_deg = K * 0.15 * ddm_loc;
    cos_value = cosf(x_deg * DegToRad);
    return x_deg / (cos_value * cos_value);
}

// вычисление отклонения по глиссаде
static float calc_ksi_g(float K, float ddm_gs)
{
    return K * 0.12 * theta * ddm_gs;
}

// проверка погрешности отклонений
static void apply_tolerance(struct ilca_output *out)
{
	if (fabsf(out->ksi_k_deg) <= allowedDegError)
        out->ksi_k_deg = 0.0;

	if (fabsf(out->ksi_g_deg) <= allowedDegError)
        out->ksi_g_deg = 0.0;

}

// проверка корректности расчета и длительности расчета
static int final_check(const struct ilca_output *out, uint32_t start_ms)
{
	if (!isfinite(out->ksi_k_deg) || !isfinite(out->ksi_g_deg))
        return 0;

	if ((uint32_t)(now_ms() - start_ms) > 20)
        return 0;

    return 1;
}

struct ilca_output ilca_calculate_deviations(const struct ilca_input *in)
{
    struct ilca_output out;
    struct ilca_amplitudes amps;
    uint32_t start_ms;
    float K;
    float ddm_loc;
    float ddm_gs;

    start_ms = now_ms();
    out.fail = false;

	if (!validate_input(in))
        return make_fail();

	if (!update_grm(in->krm_mhz, start_ms))
        return make_fail();

	if (!read_amps(&amps, start_ms))
        return make_fail();

	if (!validate_amps(&amps))
        return make_fail();

	if (!calc_k(in->distance_km, &K))
        return make_fail();

    ddm_loc = calc_ddm_loc(&amps);
    ddm_gs = calc_ddm_gs(&amps);

    out.ksi_k_deg = calc_ksi_k(K, ddm_loc);
    out.ksi_g_deg = calc_ksi_g(K, ddm_gs);

    apply_tolerance(&out);
	if (!final_check(&out, start_ms))
        return make_fail();

    return out;
}
