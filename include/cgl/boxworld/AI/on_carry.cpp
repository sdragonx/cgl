
#include "../animation.hpp"

namespace boxworld{

//������Ʒ���ֿ�
void boxAnimation::on_task_carry()
{
	if(!Units){
		return ;
	}

	boxUnits<boxUnit>* us = this->Units;
	boxUniWorld* world = us->World;
	vec2i p;
	boxUnit* item;
	if(ai.action == ACTION_IDLE){
		p = pt2map(position);
		item = world->items.find(p.x, p.y, 1024);
		if(!item){//û���ҵ���Ʒ
			setTask(TASK_IDLE);
			return ;
		}
		target = item;
		item->target = this;
		if(!this->find(item->position.x, item->position.z, 0, 0)){
			//Ŀ�겻�ɴ�
			item->target = (boxUnit*)-1;
			setTask(TASK_IDLE);
			return ;
		}
		ai.action = ACTION_MOVE_ITEM;
	}
	else if(ai.action == ACTION_MOVE_ITEM){
		if(on_move()){
			//check item state
		}
		else{
			p.x = target->position.x - position.x;
			p.y = target->position.z - position.z;
			p = p.absolute();
			//check item state
			if(p.x <= WORLD_TILE_SIZE && p.y <= WORLD_TILE_SIZE){//������Ʒ
				p = pt2map(target->position);
				world->items.tile(p.x, p.y) = null;
				if(target->debug.show_bounds){//�Ƴ�ѡ��
					world->selector.force_erase(target);
				}
				object = target;
				object->ai.animation = ANIMATION_TRANSPORT;
				target = null;
				p = pt2world(vec2i(32, 32));//�ݶ���ͼ�м��ǲֿ�
				if(!this->find(p.x, p.y, 0, 0)){//Ŀ���Ƿ�ɴ�
					//setTask(TASK_IDLE);
				}
				ai.action = ACTION_MOVE_STORE;
			}
			/*
			p = pt2map(position);
			//�����£�������Ʒ
			if(world->items.tile(p.x, p.y)){
				object = world->items.tile(p.x, p.y);
				object->scale = 0.25;
				object->target = this;
				world->items.tile(p.x, p.y) = null;

				//���Ҳֿ⣬���еص�
				//...������
				p = pt2world(vec2i(32, 32));//�ݶ���ͼ�м��ǲֿ�
				this->move(p.x, p.y);
				ai.action = ACTION_MOVE_STORE;
				CGL_LOG("move to store");
			}
			*/
			else{//��Ʒ��ʧ
				setTask(TASK_IDLE);
				CGL_LOG("item lost");
				return ;
			}
		}
	}
	else if(ai.action == ACTION_MOVE_STORE){
		if(on_move()){
			if(object){
				object->setPosition(position);
			}
		}
		else{
			p = pt2map(position);
			world->items.tile(p.x, p.y) = target;//ǿ�ƶѵ�
			object->setTile(p.x, p.y);
			object->scale = 0.5f;
			object->target = null;
			object->ai.animation = ANIMATION_IDLE;
			object = null;
			CGL_LOG("drop item");

			//��������
			setTask(TASK_IDLE);
			CGL_LOG("task end.");
		}
	}
}

}//end namespace boxworld