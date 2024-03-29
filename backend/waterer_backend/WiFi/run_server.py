#!python3

###############################################################
# Imports
###############################################################

import asyncio
import logging
from traceback import format_exc
from typing import Optional

import debugpy
from aiohttp import web
from waterer_backend.WiFi.wifi_pump_manager import PumpManagerContext
from waterer_backend.WiFi.wifi_server import create_app

###############################################################
# Definitions
###############################################################

logger = logging.getLogger(__name__)
PORT = 4000
DEBUG_PORT = 5678

RUNNER_KEY = "app_runner"


###############################################################
# Functions
###############################################################


def init_logging() -> None:

    logging.basicConfig(level=logging.INFO)
    aiohttp_logger = logging.getLogger("aiohttp")
    aiohttp_logger.setLevel(logging.WARNING)


###############################################################


def init_debugging() -> None:

    logger.info(f"Initializing debug on port: {DEBUG_PORT}")
    try:
        debugpy.listen(DEBUG_PORT)
    except RuntimeError as e:
        logger.warning(f"Failed to init_debugging with: {e}\n{format_exc()}")


###############################################################


def get_runner(app: web.Application) -> Optional[web.AppRunner]:

    if RUNNER_KEY not in app:
        return None

    runner = app[RUNNER_KEY]
    assert isinstance(runner, web.AppRunner)

    return runner


###############################################################


async def run_site(app: web.Application) -> None:

    try:
        runner = web.AppRunner(app)
        app[RUNNER_KEY] = runner

        await runner.setup()
        site = web.TCPSite(runner, port=PORT)
        logger.info(f"Starting site on: http://localhost:{PORT}")
        await site.start()
        logger.info("finished starting site")
    except asyncio.CancelledError:
        logger.info("run_site stop requested during setup ... ")


###############################################################


async def main():
    init_logging()
    init_debugging()

    async with PumpManagerContext(allow_load_history=True) as manager:

        await manager.start()
        app = create_app(manager)
        app["STOP_EVENT"] = asyncio.Event()

        #

        loop = asyncio.get_event_loop()
        task = loop.create_task(run_site(app), name="run_site")

        # wait forever
        try:
            await app["STOP_EVENT"].wait()
        except asyncio.CancelledError:
            logger.info("Stop requested ... ")
        finally:
            if not task.done():
                task.cancel()
                await task
            else:
                logger.debug("Startup task was already done")
            logger.info("Stopping webserver")

            runner = get_runner(app)
            if runner is not None:
                await runner.shutdown()
                await runner.cleanup()

            logger.info("Finished stopping webserver")


###############################################################
# Main
###############################################################

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        logger.info("User requested stop")
