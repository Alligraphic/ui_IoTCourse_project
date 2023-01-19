from requests import get
server = 'http://192.168.137.201/'

while True:
    req = input()
    get(f"{server}{req}")
