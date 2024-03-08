MQTT_PUB=build/MQTT_Pub
MQTT_PUB_SRC=src/MQTT_Pub.cpp

MQTT_SUB=build/MQTT_Sub
MQTT_SUB_SRC=src/MQTT_Sub.cpp

I2C_SRC=src/I2C/I2CDevice.cpp
I2C_INC=src/I2C/I2CDevice.h
I2C_OBJ=build/I2C/I2CDevice

ADXL345_SRC=src/ADXL345/adxl345.cpp
ADXL345_INC=src/ADXL345/adxl345.h
ADXL345_OBJ=build/ADXL345/adxl345

# Determine the operating system
UNAME_S := $(shell uname -s)

# Set the default compiler
ifeq ($(UNAME_S),Darwin)
    CC := arm-unknown-linux-gnueabihf-g++
else
    CC := g++
endif

# Other Makefile rules...
all: $(MQTT_PUB) $(MQTT_SUB)

$(MQTT_PUB): $(MQTT_PUB_SRC) $(I2C_OBJ) $(ADXL345_OBJ)
	$(CC) -g -o $(MQTT_PUB) $(MQTT_PUB_SRC) $(I2C_OBJ) $(ADXL345_OBJ) -II2CDevice -Iadxl345 -lpaho-mqtt3c

$(MQTT_SUB): $(MQTT_SUB_SRC) $(I2C_OBJ) $(ADXL345_OBJ)
	$(CC) -g -o $(MQTT_SUB) $(MQTT_SUB_SRC) $(I2C_OBJ) $(ADXL345_OBJ) -II2CDevice -Iadxl345 -lpaho-mqtt3c

$(I2C_OBJ): $(I2C_SRC) $(I2C_INC)
	$(CC) -g -c $(I2C_SRC) -o $(I2C_OBJ)

$(ADXL345_OBJ): $(ADXL345_SRC) $(ADXL345_INC) $(I2C_OBJ)
	$(CC) -g -c $(ADXL345_SRC) -o $(ADXL345_OBJ)

clean:
	rm $(MQTT_SUB)
	rm $(MQTT_PUB)
	rm $(I2C_OBJ)
	rm $(ADXL345_OBJ)

# REMOTE_USER="arun"
# REMOTE_HOST="192.168.1.200"
# REMOTE_PATH="/home/arun/Documents/assgt1"
# directory=/Users/arun/Documents/UNIVERSITY/EE513_Connected_Embedded/assignment_1/DS3231_RPI

# upload:
# 	rsync -avz --progress -e ssh $(directory) "arun@${REMOTE_HOST}:${REMOTE_PATH}"
