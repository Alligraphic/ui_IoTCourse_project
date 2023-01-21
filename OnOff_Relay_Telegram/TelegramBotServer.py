import time
import json
from requests import get, post

bot_token = '5965145269:AAHVV0UQB5x-Tb_rtcvpMFEGuK99vZSlI5M'
server_address = "192.168.137.165"
PROXIES = {
    "http": "192.168.120.150:8080",
    "https": "192.168.120.150:8080"
}
AUTH_USERS = [
    648238505
]
VALID_CMDS = [

]


def process(cmd):
    get(url=f'http://{server_address}/{cmd}')


def get_offset(update):
    return update["result"][0]["update_id"]


def main():
    while True:

        response = get(f'https://api.telegram.org/bot{bot_token}/getUpdates', proxies=PROXIES)
        if response.status_code == 200:
            res_json = response.json()
            if len(res_json["result"]) == 0:
                print("no cmds waiting...")
                pass
            elif res_json["result"][0]["message"]["from"]["id"] in AUTH_USERS:
                cmd = res_json["result"][0]["message"]["text"]
                print(f"Command : {cmd} received.")
                process(cmd)

                offset = get_offset(res_json) + 1

                get(f'https://api.telegram.org/bot{bot_token}/getUpdates?offset={offset}', proxies=PROXIES)

        time.sleep(5)


if __name__ == "__main__":
    main()
