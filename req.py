from requests import get
import time
server = 'http://192.168.137.144/'
PARAMS = {
    "On": 500,
    "Off": 500
}
while True:
    req = "LED=OFF"
    try:
        get(f"{server}{req}")
    except:
        pass
    time.sleep(1)
    req = "LED=ON"
    try:
        get(f"{server}{req}")
    except:
        pass
    # get(f"{server}", params=PARAMS)
    time.sleep(1)
