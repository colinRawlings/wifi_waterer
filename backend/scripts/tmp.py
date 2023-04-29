import aiohttp
import asyncio
import socket
from aiohttp import client_exceptions
from pydantic import error_wrappers
from time import perf_counter

import json
from waterer_backend.WiFi.wifi_smart_pump import Response


def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(("10.255.255.255", 1))
        IP = s.getsockname()[0]
    except Exception:
        IP = "127.0.0.1"
    finally:
        s.close()
    return IP


async def run_probe(
    client: aiohttp.ClientSession, ip_prefix: str, ip_suffix: int
) -> bool:

    test_ip = f"{ip_prefix}.{ip_suffix}"
    result = await _run_probe_impl(client, test_ip)

    return result


async def _run_probe_impl(client: aiohttp.ClientSession, ip: str) -> bool:

    try:
        async with client.get(f"http://{ip}/smart_pump_status") as resp:
            txt_response = await resp.text()
    except client_exceptions.ClientConnectorError as e:
        return False

    try:
        parsed_txt = json.loads(txt_response)
    except json.decoder.JSONDecodeError:
        return False

    try:
        response = Response(**parsed_txt)
    except error_wrappers.ValidationError as e:
        return False

    return True


async def run_probes():

    own_ip = get_ip()
    ip_parts = own_ip.split(".")

    ip_prefix = ".".join(ip_parts[:-1])
    ip_suffix = ip_parts[-1]

    print(f"ip_prefix: {ip_prefix}\nip_suffix: {ip_suffix}")

    T0 = perf_counter()

    async with aiohttp.ClientSession(version=aiohttp.HttpVersion11) as client:

        coros = []

        for p in range(1, 255):
            coros.append(run_probe(client, ip_prefix, p))

        results = await asyncio.gather(*coros, return_exceptions=True)

        ips = [f"{ip_prefix}.{i+1}" for i, x in enumerate(results) if x == True]

        ip_strs = "\n- ".join(ips)

        print(
            f"Found {len(ip_strs)} waterer(s) on network (in {perf_counter() - T0:.0f}s):\n- {ip_strs}"
        )


if __name__ == "__main__":
    asyncio.run(run_probes())
