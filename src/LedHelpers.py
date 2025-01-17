def meter_color(color = False):
    if color:
        return [1, 1, 1, 1, 1, 2, 2, 2, 3, 3]
    return [4, 4, 4, 4, 4, 4, 4, 4, 4, 4]

def brightness(meter):
    return [int(i * 127) for i in meter]