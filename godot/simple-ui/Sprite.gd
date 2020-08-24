extends Sprite

# class member variables go here, for example:
# var a = 2
# var b = "textvar"

func _ready():
	# Called when the node is added to the scene for the first time.
	# Initialization here
	pass

#	# Called every frame. Delta is time since last frame.
#	# Update game logic here.
func _process(delta):
	var velocity = Vector2()
	if Input.is_action_pressed("ui_right"):
	    velocity.x += 1
	if Input.is_action_pressed("ui_left"):
	    velocity.x -= 1
	if Input.is_action_pressed("ui_down"):
	    velocity.y += 1
	if Input.is_action_pressed("ui_up"):
	    velocity.y -= 1
	if velocity.length() > 0:
	    velocity = velocity.normalized() * 400

	var screensize = get_viewport_rect().size
	position += velocity * delta
	position.x = clamp(position.x, 0, screensize.x)
	position.y = clamp(position.y, 0, screensize.y)