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

/* STEP 3.3.1 - Include the header file for the Accessors library */
#include <app-common/zap-generated/attributes/Accessors.h>

LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::DeviceLayer;
/* STEP 3.1.1 - Define shorter code lines for the cluster methods and values */
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::OnOff;

/* STEP 3.3.2 - [Missing] */
constexpr EndpointId kOnOffPlugEndpointId = 1;

void ButtonEventHandler(Nrf::ButtonState state, Nrf::ButtonMask hasChanged)
{
	/* STEP 3.2.1 - If button 1 is pushed, turn on LED1 */
	if ((DK_BTN2_MSK & hasChanged) & state) {
		Nrf::PostTask([] {
			Nrf::GetBoard()
			.GetLED(Nrf::DeviceLeds::LED2)
			.Set(!Nrf::GetBoard().GetLED(Nrf::DeviceLeds::LED2).GetState());

			/* STEP 3.3.3 - Update the attribute state using the Set() method */
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

	/* STEP 3.2.2 - Pass the custom handler */
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
	/* STEP 3.1.2 - Get information about the cluster and attribute ID */
	ClusterId clusterId = attributePath.mClusterId;
	AttributeId attributeId = attributePath.mAttributeId;

	/* STEP 3.1.3 - Verify whether the modified attribute is the one we are interested in */
	if (clusterId == OnOff::Id && attributeId == OnOff::Attributes::OnOff::Id) {
		LOG_INF("Cluster OnOff: attribute OnOff set to %" PRIu8 "", *value);
		Nrf::GetBoard().GetLED(Nrf::DeviceLeds::LED2).Set(*value);
	}
}