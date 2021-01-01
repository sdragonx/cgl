//动态toolbar

class panel_t
{
public:
	static const icon_width = 50;
	static const speed = 10;
	struct icon_t
	{
		int x;	//原始位置坐标
		int cx; //当前坐标
		int dx; //目标位置坐标
	}icons[32];
	int count;
	int add_width;
	int add_id;

	panel_t()
	{
		for(int i=0; i<32; ++i){
			icons[i].x = icon_width*i;
			icons[i].cx = icons[i].x;
			icons[i].dx = icons[i].x;
		}
		add_width = 0;
		add_id = -1;
		count = 5;
	}

	void set_add_id(int id)
	{
		if(id == add_id)return ;

		add_id = id;

		for(int i=0; i<count; ++i)
		{
			if(id < 0){
				icons[i].dx = icons[i].x;
			}
			else{
				if(i < id){
					icons[i].dx = icons[i].x - add_width / 2;
				}
				else{
					icons[i].dx = icons[i].x + add_width / 2;
				}
			}
		}
    }

	void run(TPaintBox* pb)
	{
		TRect rc;

		if(mouse.y < 100){
			add_width = 100 - mouse.y;
			if(add_width >= 50)add_width = 50;
		}
		else{
			add_width = 0;
		}

		int total = icon_width * count + add_width;
		int left = (pb->ClientWidth - total)/2;

		if(mouse.y > 75){
			set_add_id(-1);
		}
		else{

			int id = (mouse.x - left) / icon_width;
			if(id < 0 || id >=count)id = -1;
			set_add_id(id);
		}

		print(IntToStr(add_id) + "\t" + add_width);

		TCanvas* dc = pb->Canvas;
		dc->Pen->Color = clRed;
		dc->Brush->Style = bsSolid;
		dc->Brush->Color = clRed;
		//dc->Rectangle(left, 0, left+total, icon_width);

		dc->Pen->Color = clGreen;
		dc->Brush->Style = bsClear;
		for(int i=0; i<count; ++i)
		{
			if(icons[i].cx != icons[i].dx){
				if(icons[i].cx < icons[i].dx){
					if(icons[i].cx + speed > icons[i].dx){
						icons[i].cx = icons[i].dx;
					}
					else{
						icons[i].cx += speed;
                    }
				}
				else{
					if(icons[i].cx - speed < icons[i].dx){
						icons[i].cx = icons[i].dx;
					}
					else{
						icons[i].cx -= speed;
                    }
				}
			}

			TRect rc = TRect(left+icons[i].cx, 0, left+icons[i].cx+icon_width, icon_width);
			dc->Rectangle(rc);
		}

		//鼠标
		rc = TRect(mouse.x - 20, mouse.y - 20, mouse.x + 20, mouse.y + 20);
		dc->Pen->Color = clBlue;
		dc->Rectangle(rc);
		dc->Ellipse(mouse.x - add_width/2, mouse.y - 20, mouse.x + add_width/2, mouse.y + 20);
    }
}panel;