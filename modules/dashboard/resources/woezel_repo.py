import time, machine, gc, term, uos, json, urequests, gc, sys, wifi, consts, ussl

#ussl.verify_letsencrypt(True) # Use Letsencrypt for the HTTPS connection

path = "/cache/woezel"
categories = []
lastUpdate = 0
updatedThisBoot = False

try:
    uos.mkdir("/cache")
except:
    pass

try:
    uos.mkdir(path)
except:
    pass

def setPath(newPath="/cache/woezel"):
    global path
    path = newPath
    categories = []
    lastUpdate = 0
    try:
        uos.mkdir(path)
    except:
        pass

def showProgress(msg, error=False, icon_wifi=False):
    term.header(True, "Installer")
    print(msg)
    icon = "/media/busy.png"
    if error:
        icon = "/media/alert.png"
    elif icon_wifi:
        icon = "/media/wifi.png"
    #easydraw.messageCentered(msg, False, icon)

def update(_showProgress=showProgress):
    global path, categories, lastUpdate, updatedThisBoot
    if not wifi.status():
        _showProgress("Connecting to WiFi...", False, True)
        wifi.connect()
        if not wifi.wait():
            _showProgress("Failed to connect to WiFi.", True, False)
            return False
    _showProgress("Downloading categories...")
    try:
        request = urequests.get("https://{}/basket/{}/categories/json".format(consts.WOEZEL_WEB_SERVER, consts.INFO_HARDWARE_WOEZEL_NAME))
        _showProgress("Saving categories...")
        categories_file = open(path+'/categories.json', 'w')
        categories_file.write(request.text)
        categories_file.close()
        _showProgress("Parsing categories...")
        categories = request.json()
        for category in categories:
            gc.collect()
            slug = category["slug"]
            _showProgress("Downloading '"+category["name"]+"'...")
            f = urequests.get("https://{}/basket/{}/category/{}/json".format(consts.WOEZEL_WEB_SERVER, consts.INFO_HARDWARE_WOEZEL_NAME, slug))
            f_file = open(path+'/'+slug+'.json', 'w')
            f_file.write(f.text)
            f_file.close()
        lastUpdate = int(time.time())
        f = open(path+"/lastUpdate", 'w')
        f.write(str(lastUpdate))
        f.close()
        _showProgress("Done!")
        updatedThisBoot = True
        gc.collect()
        return True
    except BaseException as e:
        _showProgress("Failed.", True)
        sys.print_exception(e)
        gc.collect()
    return False

def load():
    global path, categories, lastUpdate
    try:
        f = open(path+"/lastUpdate", 'r')
        data = f.read()
        f.close()
        lastUpdate = int(data)
        f = open(path+"/categories.json")
        categories = json.loads(f.read())
        f.close()
        gc.collect()
        if (lastUpdate + 900) < time.time() or not updatedThisBoot:
            # Refresh the cash if it's been at least 15 minutes since the last refresh
            # or if we're in the first 15 minutes of all time ;)
            # or if the last update is in the future
            # - We don't have RTC set up so the clock resets on each boot
            print("Current repository cache is too old!", lastUpdate + 900, "<", int(time.time()))
            return False
        return True
    except BaseException as e:
        sys.print_exception(e)
        return False

def getCategory(slug):
    global path
    f = open(path+"/"+slug+".json")
    data = json.loads(f.read())
    f.close()
    gc.collect()
    return data