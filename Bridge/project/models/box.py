import config
import random
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
          "open": self.open
      }
      print(packet)
      return str(packet)
  def simulate_box_param(self):
      self.temperature = (self.simulate_param(config.TEMP_PARAM))
      self.lock = (self.simulate_param(config.LOCK_PARAM))
      self.humidity = (self.simulate_param(config.TEMP_PARAM))
      self.infringement = self.simulate_param(config.TEMP_PARAM)
      self.presence = (self.simulate_param(config.TEMP_PARAM))
      self.open = (self.simulate_param(config.TEMP_PARAM))

  def simulate_param(self, param):
      if param == config.LOCK_PARAM:
          val = 1
      elif param == config.TEMP_PARAM:
          val = 10
      return random.randint(0, val)



