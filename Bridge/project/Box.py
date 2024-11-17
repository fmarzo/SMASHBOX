class Box:
  def __init__(self, id):
    self.id = id
    self.lock = 0
    self.temperature = 0
    self.presence = 0

  def myfunc(self):
    print("Hello my name is " + self.id)
