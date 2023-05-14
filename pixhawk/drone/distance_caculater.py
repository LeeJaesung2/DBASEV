from geopy.distance import geodesic

def distance(lat1, long1, lat2, long2):
    coord1 = (lat1, long1)
    coord2 = (lat2, long2)

    distance = geodesic(coord1, coord2).meters

    return distance