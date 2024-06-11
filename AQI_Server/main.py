import time

import numpy as np
import json

from function_AQI import post_data_to_Thingsboard, cal_aqi
from Forecast_AQI import Train_Model_Forecast_AQI, predict_AQI
import GetDataFromMQTTServer
import pandas as pd
import tensorflow as tf
from sklearn.preprocessing import MinMaxScaler

TIME_PREDICT = 3600
TIME_PRE_TRAIN_MODE = TIME_PREDICT*24


pretrain_temp = 24


while (1):
    if (pretrain_temp >= TIME_PRE_TRAIN_MODE / TIME_PREDICT):
        Train_Model_Forecast_AQI()
        pretrain_temp = 0
    predict_aqi_value = predict_AQI()
    data = {
        'aqi_predict': str(round(predict_aqi_value[0], 2)),
    }

    json_data = json.dumps(data)
    json_data = json.loads(json_data)
    response = post_data_to_Thingsboard(json_data)
    print(data)
    pretrain_temp = pretrain_temp + 1

    time.sleep(TIME_PREDICT)