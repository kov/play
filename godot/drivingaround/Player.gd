extends KinematicBody2D

export (int) var max_speed = 200
export (float) var acceleration = 40.0
export (float) var max_rotation_speed = 1.5

var velocity = Vector2()
var rotation_speed = 0.0
var rotation_accel = 0.5
var rotation_dir = 0
var speed = 0.0
var prev_rotation_dir = 0

func get_input():
	rotation_dir = 0
	velocity = Vector2()
	var delta_accel = acceleration / 60.0
	var rotation_delta = rotation_accel / 10.0

	if Input.is_action_pressed('ui_right'):
		rotation_dir += 1
	if Input.is_action_pressed('ui_left'):
		rotation_dir -= 1

	if prev_rotation_dir != rotation_dir:
		rotation_speed = 0.0
	prev_rotation_dir = rotation_dir

	if not Input.is_action_pressed('ui_right') and not Input.is_action_pressed('ui_left'):
		rotation_speed = 0.0
	else:
		rotation_speed += rotation_delta


	if rotation_speed > max_rotation_speed:
		rotation_speed = max_rotation_speed
	elif rotation_speed < 0:
		rotation_speed = 0.0

	if Input.is_action_pressed('accelerate'):
		speed = speed + delta_accel
		if speed > max_speed:
			speed = max_speed
	if Input.is_action_pressed('break'):
		speed = speed - (delta_accel * 2)
		if speed < 0:
			speed = 0
	velocity = Vector2(speed, 0).rotated(rotation)

func _physics_process(delta):
	get_input()
	rotation += rotation_dir * rotation_speed * delta
	move_and_slide(velocity)

func _ready():
	# Called when the node is added to the scene for the first time.
	# Initialization here
	pass