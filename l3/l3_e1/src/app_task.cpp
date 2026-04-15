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


LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::DeviceLayer;
/* STEP 3.1.1 - Define shorter code lines for the cluster methods and values */


/* STEP 3.3.2 - Define a helper const value used to propagate the OnOffPlug endpoint id */


void ButtonEventHandler(Nrf::ButtonState state, Nrf::ButtonMask hasChanged)
{
	/* STEP 3.2.1 - If button 1 is pushed, turn on LED1 */


}

CHIP_ERROR AppTask::Init()
{
	/* Initialize Matter stack */
	ReturnErrorOnFailure(Nrf::Matter::PrepareServer());

	/* STEP 3.2.2 - Pass the custom handler */


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


	/* STEP 3.1.3 - Verify whether the modified attribute is the one we are interested in */

}