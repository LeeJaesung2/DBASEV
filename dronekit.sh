#!/bin/bash

# Update package lists
apt-get update

# Upgrade all installed packages
apt-get upgrade

# Install required packages
apt-get install -y python3-pip
pip install dronekit dronekit-sitl