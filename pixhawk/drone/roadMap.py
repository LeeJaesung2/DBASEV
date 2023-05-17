import pandas as pd
class roadMap:
    def __init__(self) :
        self.roadmap = {}
        self.roadDf = None

        self.getData()
        self.createRoadmap()

    def getData(self):
        excel_file = 'roadData/road_data.xlsx'
        self.roadDf = pd.read_excel(excel_file)
        
    def createRoadmap(self):
        road_ids = self.roadDf['road_id'].unique()

        for road_id in road_ids:
            waypoints = []
            for index, row in self.roadDf.iterrows():
                if row['road_id'] == road_id:
                    waypoints.append([int(row['road_id']), int(row['waypoint_id']), row['lat'], row['lon'], row['alt']])
            waypoints = sorted(waypoints, key=lambda x: x[1])
            self.roadmap[road_id] = waypoints