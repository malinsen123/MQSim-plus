#include "ONFI_Channel_NVDDR2.h"

namespace SSD_Components
{
	/*
	ONFI_Channel_NVDDR2::ONFI_Channel_NVDDR2(flash_channel_ID_type channelID, unsigned int chipCount, NVM::FlashMemory::Flash_Chip** flashChips, unsigned int ChannelWidth,
		sim_time_type t_RC, sim_time_type t_DSC,
		sim_time_type t_DBSY, sim_time_type t_CS, sim_time_type t_RR,
		sim_time_type t_WB, sim_time_type t_WC, sim_time_type t_ADL, sim_time_type t_CALS,
		sim_time_type t_DQSRE, sim_time_type t_RPRE, sim_time_type t_RHW, sim_time_type t_CCS,
		sim_time_type t_WPST, sim_time_type t_WPSTH)
		: ONFI_Channel_Base(channelID, chipCount, flashChips, ONFI_Protocol::NVDDR2),
		ChannelWidth(ChannelWidth),
		t_RC(t_RC), t_DSC(t_DSC), t_DBSY(t_DBSY), t_CS(t_CS), t_RR(t_RR), t_WB(t_WB), t_WC(t_WC), t_ADL(t_ADL),
		t_CALS(t_CALS), t_DQSRE(t_DQSRE), t_RPRE(t_RPRE), t_RHW(t_RHW), t_CCS(t_CCS), t_WPST(t_WPST), t_WPSTH(t_WPSTH) //LM original end
	*/


	ONFI_Channel_NVDDR2::ONFI_Channel_NVDDR2(flash_channel_ID_type channelID, unsigned int chipCount, NVM::FlashMemory::Flash_Chip** flashChips, unsigned int ChannelWidth,
		sim_time_type t_RC, sim_time_type t_DSC, bool SCA_Enabled,
		sim_time_type t_DBSY, sim_time_type t_CS, sim_time_type t_RR,
		sim_time_type t_WB, sim_time_type t_WC, sim_time_type t_ADL, sim_time_type t_CALS,
		sim_time_type t_DQSRE, sim_time_type t_RPRE, sim_time_type t_RHW, sim_time_type t_CCS,
		sim_time_type t_WPST, sim_time_type t_WPSTH)
		: ONFI_Channel_Base(channelID, chipCount, flashChips, ONFI_Protocol::NVDDR2),
		ChannelWidth(ChannelWidth),
		t_RC(t_RC), t_DSC(t_DSC), t_DBSY(t_DBSY), t_CS(t_CS), t_RR(t_RR), t_WB(t_WB), t_WC(t_WC), t_ADL(t_ADL),
		t_CALS(t_CALS), t_DQSRE(t_DQSRE), t_RPRE(t_RPRE), t_RHW(t_RHW), t_CCS(t_CCS), t_WPST(t_WPST), t_WPSTH(t_WPSTH) //LM original end
	{

		//t_CS = 20; t_RR = 20; t_WB = 100; t_WC = 25; t_ADL = 70; t_CALS = 15;

		ReadCommandTime[1] = t_CS + t_WC * 6 + t_WB + t_RR; //LM Read command transfer time for 1 plane  = 20 + 25*6 + 100 + 20 = 290ns
		ReadCommandTime[2] = t_CS + 6 * t_WC + t_DBSY + 6 * t_WC + t_WB + t_RR;	//LM Read command transfer time for 2 planes = 20 + 25*6 + 500 + 25*6 + 100 + 20 = 940ns
		ReadCommandTime[3] = t_CS + 6 * t_WC + t_DBSY + 6 * t_WC + t_DBSY + 6 * t_WC + t_WB + t_RR; //LM Read command transfer time for 3 planes = 20 + 25*6 + 500 + 25*6 + 500 + 25*6 + 100 + 20 = 1590ns
		ReadCommandTime[4] = t_CS + 6 * t_WC + t_DBSY + 6 * t_WC + t_DBSY + 6 * t_WC + t_DBSY + 6 * t_WC + t_WB + t_RR; //LM Read command transfer time for 4 planes = 20 + 25*6 + 500 + 25*6 + 500 + 25*6 + 500 + 25*6 + 100 + 20 = 2240ns
		
		//LM SCA will separate and cancel the read command transfer time for 1 plane, 2 planes, 3 planes, and 4 planes
		if(SCA_Enabled)
		{
			ReadCommandTime[1] = 60;
			ReadCommandTime[2] = 60;
			ReadCommandTime[3] = 60;
			ReadCommandTime[4] = 60;
		}
		ReadDataOutSetupTime = t_RPRE + t_DQSRE;
		ReadDataOutSetupTime_TwoPlane = t_RPRE + t_DQSRE + t_RHW + 6 * t_WC + t_CCS + t_RPRE + t_DQSRE;
		ReadDataOutSetupTime_ThreePlane = t_RPRE + t_DQSRE + t_RHW + 6 * t_WC + t_CCS + t_RPRE + t_DQSRE + t_RHW + 6 * t_WC + t_CCS + t_RPRE + t_DQSRE;
		ReadDataOutSetupTime_FourPlane = t_RPRE + t_DQSRE + t_RHW + 6 * t_WC + t_CCS + t_RPRE + t_DQSRE + t_RHW + 6 * t_WC + t_CCS + t_RPRE + t_DQSRE + t_RHW + 6 * t_WC + t_CCS + t_RPRE + t_DQSRE;
		TwoUnitDataInTime = t_RC;

		ProgramCommandTime[1] = t_CS + t_WC * 6 + t_ADL + t_WPST + t_WPSTH + t_WB;
		ProgramCommandTime[2] = t_CS + (t_WC * 5 + t_ADL + t_WPST + t_CALS + t_WB) + t_DBSY + (t_WC * 6 + t_ADL + t_WPST + t_WPSTH + t_WB);
		ProgramCommandTime[3] = t_CS + (t_WC * 5 + t_ADL + t_WPST + t_CALS + t_WB) + t_DBSY + (t_WC * 5 + t_ADL + t_WPST + t_CALS + t_WB) + t_DBSY + (t_WC * 6 + t_ADL + t_WPST + t_WPSTH + t_WB);
		ProgramCommandTime[4] = t_CS + (t_WC * 5 + t_ADL + t_WPST + t_CALS + t_WB) + t_DBSY + (t_WC * 5 + t_ADL + t_WPST + t_CALS + t_WB) + t_DBSY + (t_WC * 5 + t_ADL + t_WPST + t_CALS + t_WB) + t_DBSY + (t_WC * 6 + t_ADL + t_WPST + t_WPSTH + t_WB);
		TwoUnitDataOutTime = t_DSC;
		ProgramSuspendCommandTime = t_CS + t_WC * 3;

		if(SCA_Enabled)
		{
			ProgramCommandTime[1] = 60;
			ProgramCommandTime[2] = 60;
			ProgramCommandTime[3] = 60;
			ProgramCommandTime[4] = 60;
			ProgramSuspendCommandTime = 60; 
		}

		EraseCommandTime[1] = t_CS + t_WC * 4 + t_WB;
		EraseCommandTime[2] = t_CS + t_WC * 4 + t_WB + (t_DBSY + t_WC * 4 + t_WB);
		EraseCommandTime[3] = t_CS + t_WC * 4 + t_WB + (t_DBSY + t_WC * 4 + t_WB) + (t_DBSY + t_WC * 4 + t_WB);
		EraseCommandTime[4] = t_CS + t_WC * 4 + t_WB + (t_DBSY + t_WC * 4 + t_WB) + (t_DBSY + t_WC * 4 + t_WB) + (t_DBSY + t_WC * 4 + t_WB);
		EraseSuspendCommandTime = t_CS + t_WC * 3;

		if(SCA_Enabled)
		{
			EraseCommandTime[1] = 60;
			EraseCommandTime[2] = 60;
			EraseCommandTime[3] = 60;
			EraseCommandTime[4] = 60;
			EraseSuspendCommandTime = 60;
		}


	}
}
