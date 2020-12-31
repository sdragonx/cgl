namespace boxworld{

void boxAnimation::on_task_cut()
{
	if(!Units){
		return ;
	}

	boxUnits<boxUnit>* us = this->Units;
	boxUniWorld* world = us->World;
	vec2i p;
	boxUnit* tree;
	vec4f rect(position.x, position.z);
	if(ai.action == ACTION_IDLE){//—∞’““ªø≈ ˜
		std::vector<boxUnit*> ls;
		rect.expand(WORLD_TILE_SIZE * 8);
		Units->select(ls, UNIT_GROUP_PLANT, rect);
		if(!ls.empty()){
			if(!this->find(ls.front()->position.x, ls.front()->position.z, 0, 0)){
				setTask(TASK_IDLE);
				return ;
			}
			this->ai.action = ACTION_MOVE_TO_OBJECT;
			this->target = ls.front();
		}

	}
	else if(ai.action == ACTION_MOVE_TO_OBJECT){
		if(on_move()){
		}
		else{
			if(unit_distance(position, target->position) < WORLD_TILE_SIZE){
				//cut
				target->ai.state = STATE_DISPOSE;//ø≥µÙ
				world->items.dropitem(target->position.x, target->position.z, "wood");
				setTask(TASK_IDLE);
			}
        }
    }
}

}//end namespace boxworld