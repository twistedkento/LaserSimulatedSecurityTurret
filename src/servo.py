class servo:

	angle = 0

	def turn_left(self):
		self.angle -= 1

	def turn_right(self):
		self.angle += 1

	def get_angle(self):
		return self.angle