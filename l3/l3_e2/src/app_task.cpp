/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "app_task.h"

#include "app/matter_init.h"
#include "app/task_executor.h"
#include "board/board.h"
#include "lib/core/CHIPError.h"
#include "lib/support/CodeUtils.h"

#include <setup_payload/OnboardingCodesUtil.h>

#include <zephyr/logging/log.h>

#include <app-common/zap-generated/attributes/Accessors.h>

/* STEP 4.5 - Include the header file that contains the data model callback declarations */


/* STEP 4.8 Include the header file that contains the method for generating random numbers */


LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::DeviceLayer;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::OnOff;
/* Add using namespace entry for the custom cluster */
using namespace ::chip::app::Clusters::RandomNumberGenerator;

constexpr EndpointId kOnOffPlugEndpointId = 1;

void ButtonEventHandler(Nrf::ButtonState state, Nrf::ButtonMask hasChanged)
{
	if ((DK_BTN2_MSK & hasChanged) & state) {
		Nrf::PostTask([] {
			Nrf::GetBoard()
			.GetLED(Nrf::DeviceLeds::LED2)
			.Set(!Nrf::GetBoard().GetLED(Nrf::DeviceLeds::LED2).GetState());

			SystemLayer().ScheduleLambda([] {
				Protocols::InteractionModel::Status status = Clusters::OnOff::Attributes::OnOff::Set(
					kOnOffPlugEndpointId, Nrf::GetBoard().GetLED(Nrf::DeviceLeds::LED2).GetState());

				if (status != Protocols::InteractionModel::Status::Success) {
					LOG_ERR("Updating on/off cluster failed: %x", to_underlying(status));
				}
			});
		});
	}
}

CHIP_ERROR AppTask::Init()
{
	/* Initialize Matter stack */
	ReturnErrorOnFailure(Nrf::Matter::PrepareServer());

	if (!Nrf::GetBoard().Init(ButtonEventHandler)) {
		LOG_ERR("User interface initialization failed.");
		return CHIP_ERROR_INCORRECT_STATE;
	}

	/* Register Matter event handler that controls the connectivity status LED based on the captured Matter network
	 * state. */
	ReturnErrorOnFailure(Nrf::Matter::RegisterEventHandler(Nrf::Board::DefaultMatterEventHandler, 0));

	return Nrf::Matter::StartServer();
}

CHIP_ERROR AppTask::StartApp()
{
	ReturnErrorOnFailure(Init());

	while (true) {
		Nrf::DispatchNextTask();
	}

	return CHIP_NO_ERROR;
}


void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath &attributePath, uint8_t type,
				       uint16_t size, uint8_t *value)
{
	ClusterId clusterId = attributePath.mClusterId;
	AttributeId attributeId = attributePath.mAttributeId;

	if (clusterId == OnOff::Id && attributeId == OnOff::Attributes::OnOff::Id) {
		LOG_INF("Cluster OnOff: attribute OnOff set to %" PRIu8 "", *value);
		Nrf::GetBoard().GetLED(Nrf::DeviceLeds::LED2).Set(*value);
	}
	/* STEP 4.11 -  Add handling the GenerateNumber attribute value changes */

}

/* STEP 4.4 - Define the Init callback for the RandomNumberGenerator cluster */


bool emberAfRandomNumberGeneratorClusterGenerateCallback(chip::app::CommandHandler *commandObj, const chip::app::ConcreteCommandPath &commandPath,
	const RandomNumberGenerator::Commands::Generate::DecodableType &commandData)
{
	/* STEP 4.7 - Obtain arguments included in the command that are min and max values for the random number generation */
	

	/* STEP 4.9 - Generate a random number in a specified range */


	/* STEP 4.10 - Set the generated number as a value of the GeneratedNumber attribute */ 

}