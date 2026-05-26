	#include "arlo.h"	
#include <string.h>	

static bool ARLO_ElapsedAtLeast(uint32_t now, uint32_t start, uint32_t timeout)	
{	
    return (uint32_t)(now - start) >= timeout;	
}	

void ARLO_Init(ArloState_t *state)	
{	
    if (state == 0) {	
        return;	
    }	

    memset(state, 0, sizeof(*state));	

    state->Target_Validity_Result = ARLO_INVALID;	
    state->Response_Acceptance_Result = ARLO_REJECTED;	
    state->Crypto_Result = ARLO_NOT_AVAILABLE;	
    state->Classification_Decision_Result = ARLO_CLASS_UNKNOWN;	
    state->Classification_Status = ARLO_CLASS_UNKNOWN;	
    state->Request_Authenticity_Result = ARLO_INVALID;	
    state->Request_Authenticity = ARLO_INVALID;	
    state->Active_Crypto_Mode = ARLO_MODE_PASSWORD;	
    state->Blanking_Decision_Result = ARLO_BLANKING_NORMAL;	
    state->Failure_Hold_Result = ARLO_HOLD_NOT_CONFIRMED;	
    state->Failure_Clear_Result = ARLO_CLEAR_NOT_CONFIRMED;	
    state->Failure_Priority_Result = ARLO_FAIL_NO_FAIL;	
    state->Compatibility_Decision_Result = ARLO_NOT_ACTIVE;	
    state->Interrogation_Message.Code_State = ARLO_CODE_NOT_READY;	
    state->Response_Message.Code_State = ARLO_CODE_NOT_READY;	
    state->Response_Message.Additional_Data_Status = ARLO_ADDITIONAL_DATA_NOT_AVAILABLE;	
    state->Weapon_Message.Release_Status = ARLO_RELEASE_NOT_PROHIBITED_BY_IFF;	
    state->Display_Message_Set = ARLO_DISPLAY_NONE;	
    state->Maintenance_Report.Failure_Code = ARLO_FAIL_NO_FAIL;	
    state->Maintenance_Report.Failure_Status = ARLO_FAILURE_STATUS_INACTIVE;	
    state->Compatibility_Message.Compatible_Mode_Status = ARLO_NOT_ACTIVE;	
}	

static void ARLO_ProcessRlsTargetData(ArloState_t *state)	
{	
    /* Обработка данных от бортовой РЛС. */	
/* При действительном результате валидности цели признак валидной цели устанавливается в истинное значение. */
