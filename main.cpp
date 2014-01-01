// First Name: Cody
// Last Name: Troyer
// email address: &lt;ctroy001@ucr.edu&gt;
//
// I hereby certify that the code in this file
// is ENTIRELY my own original work.
//
// ===========================================================

#include "instinct.h"

const double FROG_SIZE = 5.0;
const Point FROG_START = Point( 45, 0 );
const double XMOVE = FROG_SIZE;
const double YMOVE = FROG_SIZE;
const double TRUCK_W = FROG_SIZE * 4;
const double TRUCK_H = FROG_SIZE;
const double TRUCK_LANE1 = FROG_SIZE * 2;
const double TRUCK_LANE2 = FROG_SIZE * 3;
const double TRUCK_SPEED1 = TRUCK_W * 0.02;
const double TRUCK_SPEED2 = TRUCK_W * 0.025;
const double CAR_SPEED1 = TRUCK_W * 0.005;
const double CAR_SPEED2 = TRUCK_W * 0.01;
const double CAR_SPEED3 = TRUCK_W * 0.015;
const int FROG_WAIT = 100;
const int FRAME_WAIT = 20;
const int WIN_SIZE = 100;

void move_frog( Rect &amp;frog, Timer &amp;frog_tmr );
void move_trucks( Rect &amp;truck1, Rect &amp;truck2, int x );
void move_cars( Rect &amp;car1, Rect &amp;car2, Rect &amp;car3, int x);
void output_splat( Point frog );
bool collision( Rect r1, Rect r2 );
bool inside( Point p, Rect r );

int instinct_main()
{
  win.coord(0, 0, 100, 100);
  int lives = 3;
  while (lives&gt;0)
  {
    vector&lt;Rect&gt; cars(9);           //vector of cars
    vector&lt;Rect&gt; trucks(4);         //vector of trucks
    Rect rec1 = Rect(Point(0, 0), 100, 10, BLACK, true);  //black background
    Rect rec2 = Rect(Point(0, 35), 100, 65, BLACK, true);   //black background
    Rect rec3 = Rect(Point(20, 35), 5.01, 5.01, PINK, true);//left win square
    Rect rec4 = Rect(Point(75, 35), 5.01, 5.01, PINK, true);//right win square
    Rect frog = Rect(FROG_START, FROG_SIZE, FROG_SIZE* .99, BLUE, true );
    int i, j, k;
    for (i = 0, j = -TRUCK_W, k = TRUCK_LANE1; i &lt; 4; i++, k = k + 15)
    {
      if (i == 0) j = 60;             //sets starting position
      trucks[i] = Rect(Point(j, k), TRUCK_W, TRUCK_H, GREEN, true);//2 trucks per lane
      i++;
      trucks[i] = Rect(Point(j - 60, k), TRUCK_W, TRUCK_H, GREEN, true);
    }
    for (i = 0, j = win.get_coord_width(), k = TRUCK_LANE2; i &lt; 9; i++, k += 5)
    {
      if (i == 0) j = 33;            //sets starting position
      if (i &gt; 5) k += 5;
      cars[i] = Rect(Point(j, k), TRUCK_W / 2, TRUCK_H, GREEN, true);//3 cars per lane
      i++;
      cars[i] = Rect(Point(j + 33, k), TRUCK_W / 2, TRUCK_H, GREEN, true);
      i++;
      cars[i] = Rect(Point(j + 66, k), TRUCK_W / 2, TRUCK_H, GREEN, true);
    }
    Timer frog_tmr;
    frog_tmr.start();
    frog_tmr.add_time(FROG_WAIT);
    Timer frame_tmr;
    frame_tmr.start();
    for(int i = 0; i &lt; 4; i++) win &lt;&lt; trucks[i];       //displays trucks
    for(int i = 0; i &lt; 9; i++) win &lt;&lt; cars[i];       //displays cars
    win &lt;&lt; rec1 &lt;&lt; rec2 &lt;&lt; rec3 &lt;&lt; rec4 &lt;&lt; frog;

    bool decide = false;          //decides win or lose
    bool collide = false;           //collision is false
    while(!collide)
    {
      for(int i = 0; i &lt; 4; i++)
        if(collision(frog, trucks[i]))
          collide = true;       //collision is now true
      for(int i = 0; i &lt; 9; i++)
        if(collision(frog, cars[i]))
          collide = true;       //collision is now true
      if(collision(frog, rec2) &amp;&amp; (!collision(frog, rec3) || !collision(frog, rec4)))  //checks if in black space or not
        collide = true;
      if(collision(frog, rec3))
      {
        decide = true;
        collide = true;         //checks for a win
      }
      if(collision(frog, rec4))
      {
        decide = true;
        collide = true;         //checks for a win
      }
      win.clear_buffer();

      move_frog(frog, frog_tmr);
      int x;
      for(int i = 0, x = 0; i &lt; 4; i++, x++)
      {
        move_trucks(trucks[i], trucks[i + 1], x);
        i++;
      }
      for(int i = 0, x = 0; i &lt; 9; i++, x++)
      {
        move_cars(cars[i], cars[i + 1], cars[i + 2], x);
        i += 2;
      }
      //each frame occurs no faster than FRAME_WAIT milliseconds
      wait_for(FRAME_WAIT - frame_tmr.get_time());
      frame_tmr.reset();
      for(int i = 0; i &lt; 4; i++) win &lt;&lt; trucks[i];
      for(int i = 0; i &lt; 9; i++) win &lt;&lt; cars[i];
      win &lt;&lt; rec1 &lt;&lt; rec2 &lt;&lt; rec3 &lt;&lt; rec4 &lt;&lt; frog
        &lt;&lt; Message(Point(5, 90), "Lives: ", WHITE)
        &lt;&lt; Message(Point(10, 85), lives, WHITE);

      win.output_buffer();
      if(decide)
      {
        win &lt;&lt; Message(Point(47, 75), "WIN", WHITE);
        break;
      }
      else if(!decide &amp;&amp; collide)
      {
        output_splat(frog.get_lower_left());
        lives--;
      }
    }

    win.output_buffer();
    wait_for(1000);
    if(lives == 0)
    {
      win.clear_buffer();
      win &lt;&lt; Rect(Point(0, 0), 100, 100, BLACK, true);
      win &lt;&lt; Message(Point(42, 49), "GAME OVER!", WHITE);
      win.output_buffer();
    }
    if(decide) break;
  }
  return 0;
}
void output_splat(Point frog)
{
  Point ker_splat = Point(frog.get_x() - LETTER_WIDTH, frog.get_y() - LETTER_HEIGHT);
  win &lt;&lt; Message(ker_splat, "KER-SPLAT!", RED);
}

void move_frog(Rect &amp;frog, Timer &amp;frog_tmr)
{
  if(frog_tmr.has_passed(FROG_WAIT))
  {
    if(win.is_key_down("up"))
      frog.move(0, YMOVE);
    else if(win.is_key_down("left"))
      frog.move(-XMOVE, 0);
    else if(win.is_key_down("right"))
      frog.move(XMOVE, 0);
    else if(win.is_key_down("down"))
      frog.move(0, -YMOVE);
    frog_tmr.reset();
  }
   //frog can't move faster than once per FROG_WAIT milliseconds
}

void move_trucks(Rect &amp;truck1, Rect &amp;truck2, int x)
{
  if(x % 2 == 0)
  {
    truck1.move(TRUCK_SPEED1, 0);
    truck2.move(TRUCK_SPEED1, 0);
    if(truck1.get_lower_left().get_x() &gt; WIN_SIZE)
      truck1 = Rect(Point(-TRUCK_W, TRUCK_LANE1), TRUCK_W, TRUCK_H, GREEN, true);
    if (truck2.get_lower_left().get_x() &gt; WIN_SIZE)
      truck2 = Rect(Point(-TRUCK_W, TRUCK_LANE1), TRUCK_W, TRUCK_H, GREEN, true);
  }
  if (x % 2 == 1)
  {
    truck1.move(TRUCK_SPEED2, 0);
    truck2.move(TRUCK_SPEED2, 0);
    if(truck1.get_lower_left().get_x() &gt; WIN_SIZE)
      truck1 = Rect(Point(-TRUCK_W, 25), TRUCK_W, TRUCK_H, GREEN, true);
    if(truck2.get_lower_left().get_x() &gt; WIN_SIZE)
      truck2 = Rect(Point(-TRUCK_W, 25), TRUCK_W, TRUCK_H, GREEN, true);
  }
}

void move_cars(Rect &amp;car1, Rect &amp;car2, Rect &amp;car3, int x)
{
  if(x % 3 == 0)
  {
    car1.move(-CAR_SPEED1, 0);
    car2.move(-CAR_SPEED1, 0);
    car3.move(-CAR_SPEED1, 0);
    if(car1.get_lower_left().get_x() + car1.get_width() &lt; 0)
      car1 = Rect(Point(win.get_coord_width(), TRUCK_LANE2), TRUCK_W / 2, TRUCK_H, GREEN, true);
    if(car2.get_lower_left().get_x() + car2.get_width() &lt; 0)
      car2 = Rect(Point(win.get_coord_width(), TRUCK_LANE2), TRUCK_W / 2, TRUCK_H, GREEN, true);
    if(car3.get_lower_left().get_x() + car3.get_width() &lt; 0)
      car3 = Rect(Point(win.get_coord_width(), TRUCK_LANE2), TRUCK_W / 2, TRUCK_H, GREEN, true);
  }
  if(x % 3 == 1)
  {
    car1.move(-CAR_SPEED2, 0);
    car2.move(-CAR_SPEED2, 0);
    car3.move(-CAR_SPEED2, 0);
    if(car1.get_lower_left().get_x() + car1.get_width() &lt; 0)
      car1 = Rect(Point(win.get_coord_width(), TRUCK_LANE2 + 5), TRUCK_W / 2, TRUCK_H, GREEN, true);
    if(car2.get_lower_left().get_x() + car2.get_width() &lt; 0)
      car2 = Rect(Point(win.get_coord_width(), TRUCK_LANE2 + 5), TRUCK_W / 2, TRUCK_H, GREEN, true);
    if(car3.get_lower_left().get_x() + car3.get_width() &lt; 0)
      car3 = Rect(Point(win.get_coord_width(), TRUCK_LANE2 + 5), TRUCK_W / 2, TRUCK_H, GREEN, true);
  }
  if(x % 3 == 2)
  {
    car1.move(-CAR_SPEED3, 0);
    car2.move(-CAR_SPEED3, 0);
    car3.move(-CAR_SPEED3, 0);
    if(car1.get_lower_left().get_x() + car1.get_width() &lt; 0)
      car1 = Rect(Point(win.get_coord_width(), TRUCK_LANE2 + 15), TRUCK_W / 2, TRUCK_H, GREEN, true);
    if(car2.get_lower_left().get_x() + car2.get_width() &lt; 0)
      car2 = Rect(Point(win.get_coord_width(), TRUCK_LANE2 + 15), TRUCK_W / 2, TRUCK_H, GREEN, true);
    if(car3.get_lower_left().get_x() + car3.get_width() &lt; 0)
      car3 = Rect(Point(win.get_coord_width(), TRUCK_LANE2 + 15), TRUCK_W / 2, TRUCK_H, GREEN, true);
  }
}
bool inside(Point p, Rect r)
{
  double x = p.get_x();
  double y = p.get_y();
  double x1 = r.get_lower_left().get_x();
  double x2 = r.get_lower_left().get_x() + r.get_width();
  double y1 = r.get_lower_left().get_y();
  double y2 = r.get_lower_left().get_y() + r.get_height();
  return (x &gt; x1 &amp;&amp; x &lt; x2 &amp;&amp; y &gt;= y1 &amp;&amp; y &lt; y2);
}

bool collision(Rect r1, Rect r2)
{
  Point p1 = r1.get_lower_left();
  Point p2 = Point(r1.get_lower_left().get_x() + r1.get_width(), r1.get_lower_left().get_y());
  Point p3 = Point(r1.get_lower_left().get_x(), r1.get_lower_left().get_y() + r1.get_height());
  Point p4 = Point(r1.get_lower_left().get_x() + r1.get_width(), r1.get_lower_left().get_y() + r1.get_height());
  return (inside(p1, r2) || inside(p2, r2) || inside(p3, r2) || inside(p4, r2));
}
