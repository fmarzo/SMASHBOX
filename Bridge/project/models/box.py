class Box:
  def __init__(self, box_id, url_dev):
    self.id = box_id
    self.__url_device = url_dev
    self.__lock = 0
    self.__temperature = 0
    self.__presence = 0

  def set_temp(self, temp):
    self.__temperature = temp

  def get_temp(self):
      return self.__temperature

  def set_lock(self, lock):
    self.__lock = lock

  def get_lock(self):
    return self.__lock

  def set_presence(self, lock):
      self.__presence = lock

  def get_presence(self):
      return self.__presence

  def set_url_dev(self, url_dev):
      self.__url_device = url_dev

  def get_url_dev(self):
      return self.__url_device

