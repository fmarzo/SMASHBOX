from jwt.utils import bytes_from_int
from mpl_toolkits.axisartist.angle_helper import select_step_degree

import config
import random
from datetime import datetime

def simulate_param(param):
    if param == config.LOCK_PARAM:
        val = 1
    elif param == config.TEMP_PARAM:
        val = 10
    return random.randint(0, val)


class Box:
  def __init__(self, box_id, url_dev):
    self.id = box_id
    self.__url_device = url_dev
    self.lock = 0
    self.temperature = 0
    self.humidity = 0
    self.infringement = 0
    self.presence = 0
    self.open = 0
    self.date = "NULL"

  def set_url_dev(self, url_dev):
      self.__url_device = url_dev

  def get_url_dev(self):
      return self.__url_device

  def get_packet_str(self):
      packet = {
          "ID": self.id,
          "presence": self.presence,
          "temperature": self.temperature,
          "humidity": self.humidity,
          "infringement": self.infringement,
          "lock": self.lock,
          "open": self.open,
          "log": self.date
      }
      print(packet)
      return str(packet)

  def simulate_box_param(self):
      self.temperature = (simulate_param(config.TEMP_PARAM))
      self.lock = (simulate_param(config.LOCK_PARAM))
      self.humidity = (simulate_param(config.TEMP_PARAM))
      self.infringement = simulate_param(config.TEMP_PARAM)
      self.presence = (simulate_param(config.TEMP_PARAM))
      self.open = (simulate_param(config.TEMP_PARAM))

  def set_box_param(self, packet):
      self.presence = int(chr(packet[3]))
      self.temperature = (int(chr(packet[4])) * 10) + int(chr(packet [5]))
      self.humidity = (int(chr(packet[6])) * 10) + int(chr(packet[7]))
      self.infringement = int(chr(packet [8]))
      self.lock = int(chr(packet[9]))
      self.open = int(chr(packet[10]))

  def update_latest_log(self):
      formatted_timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
      print(formatted_timestamp)
      print(type(formatted_timestamp))
      self.date = formatted_timestamp






