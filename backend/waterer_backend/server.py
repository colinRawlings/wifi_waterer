#!python3

###############################################################
# Imports
###############################################################

from dataclasses import asdict

from flask import Flask, request
from flask_cors import CORS
from waterer_backend import __version__
from waterer_backend.config import get_pumps_config
from waterer_backend.pump_manager import PumpManagerContext, get_pump_manager
from waterer_backend.request import Request
from waterer_backend.service_logs import get_service_logs
from waterer_backend.smart_pump import SmartPumpSettings

###############################################################
# Routings
###############################################################


def create_app() -> Flask:
    app = Flask(__name__)
    CORS(app)

    @app.route("/")
    def main():
        return {
            "data": {
                "arduino_address": get_pump_manager().connection_info,
                "version": __version__,
            }
        }

    @app.route("/turn_on/<channel>")
    def turn_on(channel: str):
        get_pump_manager().turn_on(channel=int(channel))
        return {"data": ""}

    @app.route("/turn_on_for/<channel>", methods=["POST", "GET"])
    def turn_on_for(channel: str):
        if not request.is_json:
            raise RuntimeError("Settings should be provided as json")

        duration_s = int(request.json["duration_s"])  # type: ignore

        get_pump_manager().turn_on(channel=int(channel), duration_s=duration_s)
        return {"data": ""}

    @app.route("/turn_off/<channel>")
    def turn_off(channel: str):
        get_pump_manager().turn_off(channel=int(channel))
        return {"data": ""}

    @app.route("/status/<channel>")
    def get_pump_status(channel: str):
        status = get_pump_manager().get_status(channel=int(channel))
        return {"data": asdict(status)}

    @app.route("/clear_status/<channel>")
    def clear_status(channel: str):
        status = get_pump_manager().clear_status_logs(channel=int(channel))
        return {"data": ""}

    @app.route("/get_status_since/<channel>", methods=["POST", "GET"])
    def get_status_since(channel: str):
        if not request.is_json:
            raise RuntimeError("Settings should be provided as json")

        earliest_time = request.json["earliest_time"]  # type: ignore

        status_history = get_pump_manager().get_status_since(
            channel=int(channel), earliest_epoch_time_s=earliest_time
        )
        return {"data": asdict(status_history)}

    @app.route("/save_settings")
    def save_settings():
        saved_filepath = get_pump_manager().save_settings()
        return {"data": saved_filepath}

    @app.route("/save_history")
    def save_history():
        saved_history_dir = get_pump_manager().save_history()
        return {"data": saved_history_dir}

    @app.route("/settings/<channel>")
    def get_settings(channel: str):
        settings = get_pump_manager().get_settings(channel=int(channel))
        return {"data": asdict(settings)}

    @app.route("/set_settings/<channel>", methods=["POST", "GET"])
    def set_settings(channel: str):

        if not request.is_json:
            raise RuntimeError("Settings should be provided as json")

        new_settings = SmartPumpSettings(**request.json)  # type: ignore
        get_pump_manager().set_settings(channel=int(channel), settings=new_settings)

        settings = get_pump_manager().get_settings(channel=int(channel))
        return {"data": asdict(settings)}

    @app.route("/service_logs/<number_log_lines>")
    def service_logs(number_log_lines: str):
        logs = get_service_logs(int(number_log_lines))
        return {"data": logs}

    return app
