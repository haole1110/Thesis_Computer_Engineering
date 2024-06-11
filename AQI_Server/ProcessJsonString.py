import json

# json_str = '{"station_id":"air_0002","station_name":"NBIOT 0002","sensors":[{"id":"temp_0001","value":31.2},{"id":"humi_0001","value":64.8},{"id":"temp_0002","value":32.5},{"id":"humi_0002","value":7.7},{"id":"ph_0002","value":7},{"id":"EC_0002","value":0},{"id":"Nito_0002","value":0},{"id":"Photpho_0002","value":0},{"id":"Kali_0002","value":0},{"id":"Relay_0001","value":true},{"id":"Relay_0002","value":false},{"id":"Relay_0003","value":false},{"id":"Relay_0004","value":false}]}'
def ConversToJsonObject(json_str):
    data = json.loads(json_str)

    # Lấy giá trị từ mảng "sensors"
    sensors = data['sensors']

    # Tạo đối tượng tương ứng cho mỗi phần tử trong mảng "sensors"
    sensor_objects = []
    for sensor in sensors:
        sensor_id = sensor['id']
        sensor_value = sensor['value']
        sensor_obj = {'id': sensor_id, 'value': sensor_value}
        sensor_objects.append(sensor_obj)

    return sensor_objects