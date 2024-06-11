import pandas as pd
import numpy as np

import matplotlib.pyplot as plt
import seaborn as sns

from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
import os

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '1'
import tensorflow as tf
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense, Dropout
from tensorflow.keras.callbacks import EarlyStopping, ModelCheckpoint
from sklearn.metrics import mean_absolute_error, mean_squared_error

def Train_Model_Forecast_AQI():
    data_path = 'data_with_aqi.csv'

    # create pandas dataframe
    df = pd.read_csv(data_path)

    # remove spaces on the column
    df.columns = df.columns.str.lstrip()
    df.columns = df.columns.str.rstrip()

    # print out sample dataset
    df.head()

    # plot to see the temperature varience
    plt.figure(figsize=(15, 10))
    sns.histplot(df['AQI'],bins=[i for i in range(0,250,5)], kde=False)
    plt.title("Distribution of AQI")
    plt.grid()
    # plt.show()

    # set data index as datetime column
    df.index = pd.to_datetime(df['Date'], dayfirst=True, format='mixed')
    df = df.drop(columns=['Timestamp', 'illuminance_0001', 'atmosphere_0001', 'noise_0001', 'humi_0002', 'ph_0002', 'EC_0002', 'Nito_0002', 'temp_0001', 'humi_0001', 'CO2_0001', 'temp_0002', 'Photpho_0002', 'Kali_0002', 'vol_0001', 'vol_0002', 'power_0001'])
    # filter the columns by only the required_columns
    # 'pm10_0001', 'pm2.5_0001', 'CO_0001', 'SO2_0001', 'NO2_0001', 'O3_0001',
    required_cols = ['pm10_0001', 'pm2.5_0001', 'CO_0001', 'SO2_0001', 'NO2_0001', 'O3_0001', 'AQI']
    df = df[required_cols]
    df.head()

    sns.heatmap(df.corr(), annot=True, cmap='coolwarm')

    # check number of nan values in dataframe
    df.isna().sum()

    # fill the nan values by next valid value
    df = df.bfill()
    df.head()

    df_final = df.resample('h').mean()
    df_final.head()

    df_final.isna().sum()

    df_final = df_final.bfill()
    df_final.isna().sum()

    # Lấy 24 hàng cuối cùng
    last_24_rows = df_final.tail(24)



    # Normalize the data
    scaler = MinMaxScaler()
    scaled_data = scaler.fit_transform(df_final)

    # Define sequence length and features
    sequence_length = 24 # Number of time steps in each sequence
    num_features = len(df_final.columns)

    # Create sequences and corresponding labels
    sequences = []
    labels = []
    for i in range(len(scaled_data) - sequence_length):
        seq = scaled_data[i:i+sequence_length]
        label = scaled_data[i+sequence_length][6]  # 'AQI' column index
        sequences.append(seq)
        labels.append(label)

    # Convert to numpy arrays
    sequences = np.array(sequences)
    print(sequences)
    labels = np.array(labels)

    # Split into train and test sets
    train_size = int(0.8 * len(sequences))
    train_x, test_x = sequences[:train_size], sequences[train_size:]
    train_y, test_y = labels[:train_size], labels[train_size:]

    print("Train X shape:", train_x.shape)
    print("Train Y shape:", train_y.shape)
    print("Test X shape:", test_x.shape)
    print("Test Y shape:", test_y.shape)



    # Create the LSTM model
    model = Sequential()

    # Add LSTM layers with dropout
    model.add(LSTM(units=128, input_shape=(train_x.shape[1], train_x.shape[2]), return_sequences=True))
    model.add(Dropout(0.2))

    model.add(LSTM(units=64, return_sequences=True))
    model.add(Dropout(0.2))

    model.add(LSTM(units=32, return_sequences=False))
    model.add(Dropout(0.2))

    # Add a dense output layer
    model.add(Dense(units=1))

    # Compile the model
    model.compile(optimizer='adam', loss='mean_squared_error')

    model.summary()

    # Define callbacks
    early_stopping = EarlyStopping(monitor='val_loss', patience=30, restore_best_weights=True)
    model_checkpoint = ModelCheckpoint('layer3_seq24_32.h5', monitor='val_loss', save_best_only=True)

    # Train the model
    history = model.fit(
        train_x, train_y,
        epochs=200,
        batch_size=32,
        validation_split=0.2,  # Use part of the training data as validation
        callbacks=[early_stopping, model_checkpoint]
    )

    # Evaluate the best model on the test set
    best_model = tf.keras.models.load_model('layer3_seq24_32.h5')
    test_loss = best_model.evaluate(test_x, test_y)
    print("Test Loss:", test_loss)

    # Plot training & validation loss values
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('Model Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend(['Train', 'Validation'], loc='upper right')
    plt.show()



    # Assuming you have trained the model and have the 'best_model' object
    # Also, 'test_x' and 'test_y' should be available

    # Predict temperatures using the trained model
    predictions = best_model.predict(test_x)

    # y_true values
    test_y_copies = np.repeat(test_y.reshape(-1, 1), test_x.shape[-1], axis=-1)
    true_temp = scaler.inverse_transform(test_y_copies)[:,6]

    # predicted values
    prediction = best_model.predict(test_x)
    prediction_copies = np.repeat(prediction, 7, axis=-1)
    predicted_temp = scaler.inverse_transform(prediction_copies)[:,6]
    print(predicted_temp)

    # Calculate evaluation metrics
    mae = mean_absolute_error(true_temp, predicted_temp)
    mse = mean_squared_error(true_temp, predicted_temp)
    rmse = np.sqrt(mse)

    print("Mean Absolute Error (MAE):", mae)
    print("Mean Squared Error (MSE):", mse)
    print("Root Mean Squared Error (RMSE):", rmse)

    # Plotting predicted and actual temperatures
    plt.figure(figsize=(20, 6))
    # plt.plot(df_final.index[-1000:], true_temp[-1000:], label='Actual')
    # plt.plot(df_final.index[-1000:], predicted_temp[-1000:], label='Predicted')
    plt.plot(df_final.index[-330:], true_temp[-330:] , label='Actual')
    plt.plot(df_final.index[-330:], predicted_temp[-330:], label='Predicted')
    plt.title('AQI Prediction vs Actual')
    plt.xlabel('Time')
    plt.ylabel('AQI')
    plt.legend()
    plt.show()

def predict_AQI():
    # Đọc file CSV vào DataFrame
    df = pd.read_csv('data_with_aqi.csv')

    # remove spaces on the column
    df.columns = df.columns.str.lstrip()
    df.columns = df.columns.str.rstrip()

    # set data index as datetime column
    df.index = pd.to_datetime(df['Date'], dayfirst=True, format='mixed')
    df = df.drop(columns=['Timestamp', 'illuminance_0001', 'atmosphere_0001', 'noise_0001', 'humi_0002', 'ph_0002', 'EC_0002', 'Nito_0002', 'temp_0001', 'humi_0001', 'CO2_0001', 'temp_0002', 'Photpho_0002', 'Kali_0002', 'vol_0001', 'vol_0002', 'power_0001'])
    # filter the columns by only the required_columns
    # 'pm10_0001', 'pm2.5_0001', 'CO_0001', 'SO2_0001', 'NO2_0001', 'O3_0001',
    required_cols = ['pm10_0001', 'pm2.5_0001', 'CO_0001', 'SO2_0001', 'NO2_0001', 'O3_0001', 'AQI']
    df = df[required_cols]
    df.head()

    # check number of nan values in dataframe
    df.isna().sum()

    # fill the nan values by next valid value
    df = df.bfill()
    df.head()

    df_final = df.resample('h').mean()
    df_final.head()

    df_final.isna().sum()

    df_final = df_final.bfill()
    df_final.isna().sum()

    # Lấy 24 hàng cuối cùng
    last_24_rows = df_final.tail(24)


    scaler = MinMaxScaler()
    scaled_data = scaler.fit_transform(last_24_rows)

    # Define sequence length and features
    sequence_length = 24  # Number of time steps in each sequence
    num_features = len(df_final.columns)

    # Create sequences and corresponding labels
    sequences = []
    for i in range(len(scaled_data) - sequence_length + 1):
        seq = scaled_data[i:i + sequence_length]
        sequences.append(seq)

    # Convert to numpy arrays
    sequences = np.array(sequences)


    best_model = tf.keras.models.load_model('layer3_seq24_32.h5')
    predictions = best_model.predict(sequences)
    prediction_copies = np.repeat(predictions, 7, axis=-1)
    predicted_temp = scaler.inverse_transform(prediction_copies)[:,6]

    return predicted_temp