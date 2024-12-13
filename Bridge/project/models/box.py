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

