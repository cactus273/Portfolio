'''
	Final (9 May 2024)
'''
import pygame
import random

class Node:
	def __init__(self, data=None):
		self.data = data
		self.next = None

class LinkedList:
	def __init__(self):
		self.head = None

	def append(self, data):
		if not self.head:
			self.head = Node(data)
		else:
			current = self.head
			while current.next:
				current = current.next
			current.next = Node(data)

	def remove(self, node_value):
		if self.head is None:
			return

		if self.head.data == node_value:
			self.head = self.head.next
			return

		current = self.head
		while current.next:
			if current.next.data == node_value:
				current.next = current.next.next
				return
			current = current.next

	def __iter__(self):
		node = self.head
		while node is not None:
			yield node.data
			node = node.next

class Button:
	def __init__(self, x, y, width, height, text):
		self.rect = pygame.Rect(x, y, width, height)
		self.text = text

	def draw(self, window):
		pygame.draw.rect(window, (255, 255, 255), self.rect, 2)
		font = pygame.font.Font(None, 36)
		text = font.render(self.text, True, (255, 255, 255))
		window.blit(text, (self.rect.x + (self.rect.width - text.get_width()) // 2, self.rect.y + (self.rect.height - text.get_height()) // 2))


# Define arrow colors
ARROW_COLORS = {
	"up": (255, 0, 0),      # Red
	"down": (0, 255, 0),    # Green
	"left": (0, 0, 255),    # Blue
	"right": (255, 255, 0)  # Yellow
}

# Define arrow sizes
ARROW_SIZE = 50

MESSAGE_EVENT = pygame.USEREVENT + 1

EASY = 5
MEDIUM = 9
HARD = 12

# Initialize Pygame
pygame.init()
pygame.mixer.init()


# Set up the window
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600
window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Catch the Squares!")

# Create a font object
font = pygame.font.Font(None, 24)

# Create a surface for the button
button_surface = pygame.Surface((50, 50))

# Fill the button surface with red color
button_surface.fill((255, 0, 0))

# Render text on the button
text = font.render("X", True, (0, 0, 0))
text_rect = text.get_rect(center=(button_surface.get_width()/2, button_surface.get_height()/2))

# Create a pygame.Rect object that represents the button's boundaries
button_rect = pygame.Rect(WINDOW_WIDTH - 50, 0, 50, 50)

# Set up game variables
clock = pygame.time.Clock()
arrows = LinkedList()
score = 0
misses = 0
high_score = 0
game_start = False
game_over = False

catch_sound = pygame.mixer.Sound('sounds/catch.mp3')
miss_sound = pygame.mixer.Sound('sounds/miss.mp3')
combo_sound = pygame.mixer.Sound('sounds/combo.mp3')

# Define the lane width
lane_width = WINDOW_WIDTH // 4
# Define the box size
box_width = lane_width - 20  # 20 pixels less than the lane width for some padding
box_height = 50  # Arbitrary height

message = None

def show_starting_page(window):
	# Create buttons for each difficulty level
	easy_button = Button(50, 400, 200, 50, "Easy")
	medium_button = Button(300, 400, 200, 50, "Medium")
	hard_button = Button(550, 400, 200, 50, "Hard")
	font = pygame.font.Font(None, 36)
	title_font = pygame.font.Font(None, 64)

	running = True
	while running:
		window.fill((0, 0, 0))  # Clear the screen
		easy_button.draw(window)
		medium_button.draw(window)
		hard_button.draw(window)
  
		title_text = title_font.render("Catch the Squares!", True, (0, 200, 180))
		title_rect = title_text.get_rect(center=(WINDOW_WIDTH / 2, 130))
		window.blit(title_text, title_rect)
  
		instruction = font.render("Select a difficulty level to start the game.", True, (200, 200, 200))
		instruction_rect = instruction.get_rect(center=(WINDOW_WIDTH / 2, 200))
		window.blit(instruction, instruction_rect)
  
		how_to_play = font.render("Use the corresponding arrow keys to catch the squares!", True, (200, 200, 200))
		how_to_play_rect = how_to_play.get_rect(center=(WINDOW_WIDTH / 2, 240))
		window.blit(how_to_play, how_to_play_rect)
  
		game_over_text = font.render("5 Misses and it's Game Over...", True, (255, 10, 10))
		game_over_rect = game_over_text.get_rect(center=(WINDOW_WIDTH / 2, 500))
		window.blit(game_over_text, game_over_rect)
  
		pygame.display.update()  # Update the screen

		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				pygame.quit()
				sys.exit()
			elif event.type == pygame.MOUSEBUTTONDOWN:
				if event.button == 1:
					if easy_button.rect.collidepoint(event.pos):
						return EASY
					elif medium_button.rect.collidepoint(event.pos):
						return MEDIUM
					elif hard_button.rect.collidepoint(event.pos):
						return HARD
					
def draw_message(window, message):
	if message:
		font = pygame.font.Font(None, 36)
		text = font.render(message, True, (255, 255, 255))
		window.blit(text, (WINDOW_WIDTH / 2 - text.get_width() // 2, WINDOW_HEIGHT / 2))


def handle_events(misses, score, running, message, combo):
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			running = False
		elif event.type == pygame.MOUSEBUTTONDOWN:
			if event.button == 1:
				if button_rect.collidepoint(event.pos):
					running = False
					pygame.quit()
		elif event.type == pygame.KEYDOWN:
			if event.key in [pygame.K_LEFT, pygame.K_DOWN, pygame.K_UP, pygame.K_RIGHT]:
				# Determine the lane based on the key
				if event.key == pygame.K_LEFT:
					lane = 0
				elif event.key == pygame.K_DOWN:
					lane = 1
				elif event.key == pygame.K_UP:
					lane = 2
				elif event.key == pygame.K_RIGHT:
					lane = 3

				# Draw arrows
				for arrow in arrows:
					arrow_rect = pygame.Rect(arrow["x"] - ARROW_SIZE/2, arrow["y"] - ARROW_SIZE/2, ARROW_SIZE, ARROW_SIZE)
					if arrow['x'] // lane_width == lane:
						box_x = lane * lane_width + 10
						box_y = WINDOW_HEIGHT - box_height - 10
						box_rect = pygame.Rect(box_x, box_y, box_width, box_height)
						arrow_rect = pygame.Rect(arrow['x'] - ARROW_SIZE/2, arrow['y'] - ARROW_SIZE/2, ARROW_SIZE, ARROW_SIZE)
						if box_rect.colliderect(arrow_rect):
							score += 1
							combo += 1
							message = "Perfect!"
							catch_sound.play()

							if combo >= 3:
								message = f"Perfect! Combo: {combo}"
								# Combo multiplier of 10
								if (combo%10) == 0:
									combo_sound.play()
        
							pygame.time.set_timer(MESSAGE_EVENT, 1000)
							print(f"score: {score}")
							print(f"misses: {misses}")
							arrow['tapped'] = True
							arrows.remove(arrow)
							break  # Exit the loop as soon as one arrow is found
						else:
							misses += 1
							combo = 0
							message = "Miss!"
							miss_sound.play()
							pygame.time.set_timer(MESSAGE_EVENT, 1000)
							print(f"tapped score: {score}")
							print(f"misses: {misses}")
							break
		
		elif event.type == MESSAGE_EVENT:
			message = None
			pygame.time.set_timer(MESSAGE_EVENT, 0)
						
		# Check for the mouse button down event
		if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
			# Call the on_mouse_button_down() function
			if button_rect.collidepoint(event.pos):
				print("Button clicked!")
				running = False
				
	return misses, score, running, message, combo

def create_button(window, button_surface, text, button_rect):
	# Check if the mouse is over the button. This will create the button hover effect
	if pygame.display.get_init() and button_rect.collidepoint(pygame.mouse.get_pos()):
		pygame.draw.rect(button_surface, (127, 255, 212), (1, 1, 148, 48))
	else:
		pygame.draw.rect(button_surface, (0, 0, 0), (0, 0, 150, 50))
		pygame.draw.rect(button_surface, (255, 255, 255), (1, 1, 148, 48))
		pygame.draw.rect(button_surface, (0, 0, 0), (1, 1, 148, 1), 2)
		pygame.draw.rect(button_surface, (0, 100, 0), (1, 48, 148, 10), 2)

	# Show the button text
	button_surface.blit(text, text_rect)

	# Draw the button on the screen
	window.blit(button_surface, (button_rect.x, button_rect.y))
	
def create_box(window):
	# Define the text for each box
	box_texts = ["left", "down", "up", "right"]
	
	# Load the images
	arrow_images = {
		"up": pygame.transform.scale(pygame.image.load("images/up_arrow.png"), (50, 50)),
		"down": pygame.transform.scale(pygame.image.load("images/down_arrow.png"), (50, 50)),
		"left": pygame.transform.scale(pygame.image.load("images/left_arrow.png"), (50, 50)),
		"right": pygame.transform.scale(pygame.image.load("images/right_arrow.png"), (50, 50))
	}

	# Draw the boxes and the text
	for i in range(4):
		box_x = (i * lane_width) + 10  # 10 pixels padding from the left side of the lane
		box_y = WINDOW_HEIGHT - box_height - 10  # 10 pixels padding from the bottom of the window
		pygame.draw.rect(window, (255, 255, 255), (box_x, box_y, box_width, box_height), 2)
		
		# Draw the arrow image on top of the rectangle
		arrow_image = arrow_images[box_texts[i]]
		arrow_image_rect = arrow_image.get_rect(center=(box_x + 50/2, box_y + 50/2))
		window.blit(arrow_image, arrow_image_rect)
		
 
def show_game_over(window, score, high_score):
	replay_button = Button(100, 400, 200, 50, "Play Again")
	quit_button = Button(500, 400, 200, 50, "Quit")
	font = pygame.font.Font(None, 36)
	title_font = pygame.font.Font(None, 64)

	running = True
	while running:
		window.fill((0, 0, 0))  # Clear the screen
		replay_button.draw(window)
		quit_button.draw(window)
  
		thankyou_text = title_font.render("Thank you for playing!", True, (255, 255, 255))
		thankyou_rect = thankyou_text.get_rect(center=(WINDOW_WIDTH / 2, 130))
		window.blit(thankyou_text, thankyou_rect)
		score_text = font.render(f"Your score: {score}", True, (255, 255, 255))
		score_rect = score_text.get_rect(center=(WINDOW_WIDTH / 2, 200))
		window.blit(score_text, score_rect)
		high_score_text = font.render(f"High score: {high_score}", True, (255, 255, 255))
		high_score_rect = high_score_text.get_rect(center=(WINDOW_WIDTH / 2, 250))
		window.blit(high_score_text, high_score_rect)
  
		pygame.display.update()

		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				pygame.quit()
				sys.exit()
			elif event.type == pygame.MOUSEBUTTONDOWN:
				if event.button == 1:
					if quit_button.rect.collidepoint(event.pos):
						pygame.quit()
					elif replay_button.rect.collidepoint(event.pos):
						return True                        

    
def play(misses, score, message, arrows, high_score):
	combo = 0

	# Show the starting page before the game loop
	arrow_speed = show_starting_page(window) 
	# Game loop
	running = True
	while running:
		# Clear the window
		window.fill((0, 0, 0))

		# Draw the lanes
		for i in range(1, 4):
			pygame.draw.line(window, (255, 255, 255), (i * lane_width, 0), (i * lane_width, WINDOW_HEIGHT), 2)

		# Handle events
		handled = handle_events(misses, score, running, message, combo)
		misses = handled[0]
		score = handled[1]
		running = handled[2]
		message = handled[3]
		combo = handled[4]

		create_button(window, button_surface, text, button_rect)

		create_box(window)

		# Update the game state
		pygame.display.update()

		# Generate new arrows
		if random.random() < 0.01:
			arrow_type = random.choice(["up", "down", "left", "right"])
			lane = random.randint(0, 3)  # Choose a random lane
			arrow_x = (lane * lane_width) + (lane_width // 2)  # Calculate the x-coordinate for the middle of the lane
			arrow_y = -ARROW_SIZE
			new_arrow = {
				"type": arrow_type,
				"x": arrow_x,
				"y": arrow_y,
				"tapped": False
			}

			# Check if the new arrow would overlap with any existing arrow
			new_arrow_rect = pygame.Rect(new_arrow["x"] - ARROW_SIZE/2, new_arrow["y"] - ARROW_SIZE/2, ARROW_SIZE, ARROW_SIZE)
			for arrow in arrows:
				arrow_rect = pygame.Rect(arrow["x"] - ARROW_SIZE/2, arrow["y"] - ARROW_SIZE/2, ARROW_SIZE, ARROW_SIZE)
				if arrow_rect.colliderect(new_arrow_rect):
					# The new arrow would overlap with an existing arrow, so don't add it
					break
			else:
				# The new arrow wouldn't overlap with any existing arrow, so add it
				arrows.append(new_arrow)

		# Update arrow positions
		for arrow in arrows:
			arrow["y"] += arrow_speed
			if arrow["y"] > WINDOW_HEIGHT:
				arrows.remove(arrow)
				misses += 1
				combo = 0
				message = "Miss!"
				miss_sound.play()
				pygame.time.set_timer(MESSAGE_EVENT, 1000)
				

		# Draw arrows
		for arrow in arrows:
			arrow_color = ARROW_COLORS[arrow["type"]]
			arrow_rect = pygame.Rect(arrow["x"] - ARROW_SIZE/2, arrow["y"] - ARROW_SIZE/2, ARROW_SIZE, ARROW_SIZE)
			pygame.draw.rect(window, arrow_color, arrow_rect)

		# Draw the message
		draw_message(window, message)
		
		# Update the display
		pygame.display.flip()

		# Limit the frame rate
		clock.tick(60)
  
		# Check if the game is over
		if misses >= 5:
			if high_score < score:
					high_score = score
			running = show_game_over(window, score, high_score)
			if running:
				play(misses = 0, score = 0, message = None, arrows = LinkedList(), high_score = high_score)
			else:
				pygame.quit()
				break
		
	# Clean up Pygame
	pygame.quit()
	
 
 
# Play the Game
play(misses, score, message, arrows, high_score)
