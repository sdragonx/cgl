
namespace boxworld{

bool boxAnimation::on_follow()
{
	if(!check_target_state()){
		return false;
	}

	float distance = unit_distance(position, target->position);
	float follow_distance = vision * 0.5f;

	if(distance > follow_distance){
		if(engine.clock() - last_follow_time > 1000){
//			this->move(target->position.x + math::random(-follow_distance, follow_distance),
//				target->position.z + math::random(-follow_distance, follow_distance));
			//this->find(target->position.x + math::random(-512, 512), target->position.z + math::random(-512, 512), 0, 0);
			findpath_task(target->position.x + math::random(-512, 512), target->position.z + math::random(-512, 512));
			last_follow_time = engine.clock();
			//CGL_LOG("on follow %f", distance);
		}
	}

	if(ai.animation == ANIMATION_MOVE){
		on_move();
	}
	else{
		if(Box){
			position_set(position, Box->position());
		}
        dest = position;
    }
	//CGL_LOG("on_move");

	return true;
}

}//end namespace boxworld