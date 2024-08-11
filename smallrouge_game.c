
int entry(int argc, char **argv) {
	
	window.title = STR("Minimal Game Example");
	window.scaled_width = 1280; // We need to set the scaled size if we want to handle system scaling (DPI)
	window.scaled_height = 720; 
	window.x = 200;
	window.y = 90;
	window.clear_color = hex_to_rgba(0xA94064ff);

	Gfx_Image* player = load_image_from_disk(fixed_string("art/guy.png"), get_heap_allocator());
	assert(player, "You dun goofed no player image found")

	Vector2 player_pos = v2(0,0);
	Vector2 offset = v2(-0.5, 0.0);
	Vector2 player_size = v2(8.0,8.0);
	float zoom = 5.3;
	float move_speed = 16.0;

	float64 seconds_counter = 0.0;
	s32 frame_count = 0;
	

	float64 last_time = os_get_current_time_in_seconds();
	while (!window.should_close) {
		reset_temporary_storage();
		
		//Override drawframe
		draw_frame.projection = m4_make_orthographic_projection(window.width * -0.5,
																window.width * 0.5,
																window.height * -0.5,
																window.height * 0.5, -1, 10);

		draw_frame.view = m4_make_scale(v3(1.0/zoom, 1.0/zoom, 1.0));
		
		float64 now = os_get_current_time_in_seconds();
		float64 delta_t = now - last_time;
		last_time = now;
		
		
		os_update(); 
		
		if(is_key_just_pressed(KEY_ESCAPE)){
			window.should_close = true;
		}
		Vector2 input_axis = v2(0,0);
		if(is_key_down('W')){
			input_axis.y += 1.0f;
		}
		if(is_key_down('S')){
			input_axis.y -= 1.0f;
		}
		if(is_key_down('A')){
			input_axis.x -= 1.0f;
		}
		if(is_key_down('D')){
			input_axis.x += 1.0f;
		}

		input_axis = v2_normalize(input_axis);

		//C-style player_pos += (input_axis * 0.0001f)
		player_pos = v2_add(player_pos, v2_mulf(input_axis, move_speed * delta_t));
		
		Matrix4 xform = m4_scalar(1.0);
		xform = m4_translate(xform, v3(player_pos.x, player_pos.y, 0));
		xform = m4_translate(xform, v3(player_size.x * offset.x, offset.y, 0));
		draw_image_xform(player, xform, player_size, COLOR_WHITE);

		
		gfx_update();
		seconds_counter += delta_t;
		frame_count += 1;
		if(seconds_counter > 1.0){
			log("fps %i", frame_count);
			seconds_counter = 0.0;
			frame_count = 0;
		}
	}

	return 0;
}