# This file takes as input a .txt file with format:
# timestamp x y polarity
# and generates a .raw file compatible with Prophesee Library

# Libraries
import pandas as pd
from time import time

# Defines
EVT_TIME_HIGH = 8

# Start timer
start_time = time()

# TXT file
txtPath = "../poster_6dof/events.txt"

# Open TXT and create RAW file
txtFile = pd.read_csv(txtPath, sep=" ", header=None)

# Max index to print the progress percentage and threshold to print it
max_index = len(txtFile.index)
index_threshold = max_index/20

rawFile = open("../output_raw.raw", "wb")  # Write mode

# Write DAT header
rawFile.write(b"% date 2021-11-18 18:25:39\n"
              b"% evt 2.0\n"
              b"% firmware_version 3.5.0\n"
              b"% integrator_name Prophesee\n"
              b"% plugin_name hal_plugin_gen31_fx3\n"
              b"% serial_number 00001821\n"
              b"% subsystem_ID 0\n"
              b"% system_ID 28\n")

# Set timebase
init_time_base = int(EVT_TIME_HIGH << 28).to_bytes(4, 'little')
prev_time_base = 0
rawFile.write(init_time_base)

# Starting conversion
print("Conversion started")
for index, row in txtFile.iterrows():
    if index > index_threshold:
        print("{}% of the rows have been iterated".format(int((index + 1)/max_index * 100)))
        index_threshold += max_index/20

    timestamp = int(row[0] * 1000000)
    timestamp_word = timestamp & 0x3F
    x = int(row[1])
    y = int(row[2])
    p = int(row[3])

    if timestamp - prev_time_base >= 64:
        current_time_base_word = ((EVT_TIME_HIGH << 28) + (timestamp >> 6)).to_bytes(4, 'little')
        prev_timestamp = timestamp
        rawFile.write(current_time_base_word)

    # Assemble word
    word = (p << 28) + (timestamp_word << 22) + (x << 11) + y
    word_1 = (word & 0xFF000000) >> 24
    word_byte_1 = word_1.to_bytes(1, 'little')
    word_2 = (word & 0xFF0000) >> 16
    word_byte_2 = word_2.to_bytes(1, 'little')
    word_3 = (word & 0xFF00) >> 8
    word_byte_3 = word_3.to_bytes(1, 'little')
    word_4 = word & 0xFF
    word_byte_4 = word_4.to_bytes(1, 'little')

    # Write words
    rawFile.write(word_byte_4)
    rawFile.write(word_byte_3)
    rawFile.write(word_byte_2)
    rawFile.write(word_byte_1)

# Close raw file
rawFile.close()

# Stop timer
elapsed_time = time() - start_time
elapsed_time_hours = int(elapsed_time / 3600)
elapsed_time_minutes = int(elapsed_time / 60) - elapsed_time_hours * 60
elapsed_time_seconds = int(elapsed_time % 60)
print("Conversion finished")
print("Process took %i minutes %i seconds." % (elapsed_time_minutes, elapsed_time_seconds))
