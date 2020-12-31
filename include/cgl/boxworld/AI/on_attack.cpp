
namespace boxworld{

bool boxAnimation::on_attack()
{
	if(!check_target_state()){
		return false;
	}

	if(target->ai.state == STATE_DEADTH){
		//setTask(TASK_IDLE);
		setAction(ACTION_IDLE);
		return false;
	}

	float distance = unit_distance(position, target->position);
	float attack_range = weapon.type == WEAPON_SWORD ? WORLD_TILE_SIZE + HALF_TILE_SIZE : vision;

	if(distance > attack_range){
		if(engine.clock() - last_follow_time > 1000){
			//this->find(target->position.x, target->position.z, 0, 0);
			if(ai.action != ACTION_FIND_PATH){
				findpath_task(target->position.x, target->position.z);
				last_follow_time = engine.clock();
				//CGL_LOG("on attack %f", distance);
			}
		}
		else{
			on_move();
        }
	}
	else{
		if(Box){//同步位置信息
			position_set(position, Box->position());
		}
		dest = position;

		if(weapon.type == WEAPON_SWORD){
			this->look(target->position);
			int cd = cooldown + weapon.cooldown;//cd时间
			weapon.sword_on_attack(this->face());
			if(engine.clock() - last_attack_time >= cd){
				boxAnimation* ani = dynamic_cast<boxAnimation*>(target);
				playsound(position, "sfx/Melee_Animal_Big_HitPawn_1a.wav");
				if(ani){
					ani->hit(10);
					if(ani->ai.state == STATE_DEADTH){
						setAction(ACTION_IDLE);
						target = null;
					}
				}
				last_attack_time = engine.clock();
			}

		}
		else if(weapon.type == WEAPON_RIFLE){
			this->shoot(target->position);
		}
	}
	return true;
}

}//end namespace boxworld