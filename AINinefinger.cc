#include "Player.hh"
#include <algorithm>
#include <stack> 
#include <queue>
#include <vector>
#include <cmath>
#include <map>



#define PLAYER_NAME Ninefinger



struct PLAYER_NAME : public Player
{

  static Player *factory()
  {
    return new PLAYER_NAME;
  }

  typedef vector<int> VI;
  vector<Pos> cities; 
  map<int, vector<Pos> > orco;

  struct node
  {
    Pos pos;
    Pos parent;
    int f; //the sum of the distance from the goal to succcessor
    int g; // the sum of the cost of the current plus the one from the successor
    int h; //distance from goal to successor

    friend bool operator<(node left, node right)
    {
      return (left.f < right.f);
    }
  };

  // Returns true if winning.
  bool winning()
  {
    for (int pl = 0; pl < nb_players(); ++pl)
      if (pl != me() and total_score(me()) <= total_score(pl))
        return false;
    return true;
  }

  
    int getcost(Pos coordinates)
  {
    Cell c = cell(coordinates);
    if (c.type == GRASS)
      return 10;
    else if (c.type == SAND)
      return 50;
    else if (c.type == CITY)
      return 0;
    else if (c.type == PATH)
      return 0;
    else if (c.type == WATER)
      return 800000;
    else if (c.type == FOREST)
      return 40;
    return 0;
  }


    int getcost2(Pos coordinates)
  {
    Cell c = cell(coordinates);
    if (c.type == GRASS)
      return 200000;
    else if (c.type == SAND)
      return 500000;
    else if (c.type == CITY)
      return 0;
    else if (c.type == PATH)
      return 0;
    else if (c.type == WATER)
      return 800000;
    else if (c.type == FOREST)
      return 400000;
    return 0;
  }

  
  int heuristic(Pos origen, Pos desti){
    return abs(origen.i - desti.i) + abs(origen.j - desti.j);
  }

  node checkposition(Pos a, set<node> b){
    for (auto it = b.begin(); it != b.end(); ++it)
    {
      if ((*it).pos == a)
        return *it;
    }
    node c;
    c.f = -1;
    return c;
  }



  Dir dime_dir(Pos origen, Pos desti){
    int x = (desti.i - origen.i);
    int y = (desti.j - origen.j);
    if (x == 1 and y == 0)
      return BOTTOM;
    else if (x == -1 and y == 0)
      return TOP;
    else if (x == 0 and y == 1)
      return RIGHT;
    else if(x == 0 and y==-1)
      return LEFT;
    else 
      return NONE;
      
  }



Dir perseguirenemics(Pos pos){
  Cell c = cell(pos);
  int cityid = c.city_id;
  vector<Pos> info =  city(cityid);
  //anem a mirar si ja hi ha algu del meu exèrcit
  for(int i = 0; i < info.size(); ++i){
     if(unit(cell(info[i]).unit_id).player != me()) return dime_dir(pos,info[i]);
  }
 return NONE;
}


Pos pick_cityrandom(){

      return cities[random(0,cities.size())];
}

bool perritoguardian(Pos pos){
  Cell c = cell(pos);
  int cityid = c.city_id;
  vector<Pos> info =  city(cityid);
  //anem a mirar si ja hi ha algu del meu exèrcit
  for(int i = 0; i < info.size(); ++i){
     if(unit(cell(info[i]).unit_id).player == me()) return false;
  }
  return true;

}

stack <Pos> makepath(node infonodes[70][70], Pos desti){

    Pos pos;
    int row = desti.i;
    int col = desti.j;
    stack<Pos> Path;
     bool a = true;
    while(a){
      pos.i = row;
      pos.j = col;
      Path.push(pos);
      
      int temp_row = infonodes[row][col].parent.i;
      int temp_col = infonodes[row][col].parent.j;
      if(row == temp_row and col == temp_col) a = false;
      row = temp_row;
      col = temp_col;
    
    }

    pos.i = row;
    pos.j = col;
    Path.push(pos);
    return Path;
}




stack<Pos> search2(Pos inicio, Pos desti)
  {

    set<node> opennodes;
    Pos posicio;
    posicio.i = -1;
    posicio.j = -1;
    node infonodes[70][70];
    bool closednodes[70][70];
    memset(closednodes,false,sizeof(closednodes));
    for(int i = 0;i<70;++i){
      for(int j = 0;j<70;++j){
        infonodes[i][j].f= 100000;
        infonodes[i][j].g= 100000;
        infonodes[i][j].h= 100000;
        infonodes[i][j].parent =posicio; 
    }
  }
    
    node inici;
    node successor;
    inici.pos = inicio;
    inici.parent = inicio;
    inici.h = 0;
    inici.g = 0; 
    inici.f = 0;
    opennodes.insert(inici);
    closednodes[inicio.i][inicio.j] = true;
    infonodes[inicio.i][inicio.j] = inici;


    
    while (not opennodes.empty())
    {
      node current = *(opennodes.begin());
      opennodes.erase(opennodes.begin());
      closednodes[current.pos.i][current.pos.j] = true;
    
      Dir dir;
      for (int d = 0; d != DIR_SIZE; ++d)
      {
        dir = Dir(d);
        successor.pos = current.pos + dir;
        
        if (pos_ok(successor.pos))
        {
        
          if (successor.pos == desti)
        {
          infonodes[successor.pos.i][successor.pos.j].parent = current.pos;   
          return makepath(infonodes,successor.pos);
        }
          node n1 = checkposition(successor.pos, opennodes);
          if(closednodes[successor.pos.i][successor.pos.j]) continue;
          else if( n1.f == -1 or current.f > n1.f ) {
          successor.parent = current.pos;
          successor.g = current.g + getcost2(successor.pos);
          successor.h = heuristic(successor.pos, desti);
          successor.f = successor.g + successor.h;
          infonodes[successor.pos.i][successor.pos.j] = successor;
          opennodes.insert(successor); 
         }
        }
      }
    }
  } 





stack<Pos> search(Pos inicio, Pos desti)
  {

    set<node> opennodes;
    Pos posicio;
    posicio.i = -1;
    posicio.j = -1;
    node infonodes[70][70];
    bool closednodes[70][70];
    memset(closednodes,false,sizeof(closednodes));
    for(int i = 0;i<70;++i){
      for(int j = 0;j<70;++j){
        infonodes[i][j].f= 100000;
        infonodes[i][j].g= 100000;
        infonodes[i][j].h= 100000;
        infonodes[i][j].parent =posicio; 
    }
  }
    
    node inici;
    node successor;
    inici.pos = inicio;
    inici.parent = inicio;
    inici.h = 0;
    inici.g = 0; 
    inici.f = 0;
    opennodes.insert(inici);
    closednodes[inicio.i][inicio.j] = true;
    infonodes[inicio.i][inicio.j] = inici;


    
    while (not opennodes.empty())
    {
      node current = *(opennodes.begin());
      opennodes.erase(opennodes.begin());
      closednodes[current.pos.i][current.pos.j] = true;
    
      Dir dir;
      for (int d = 0; d != DIR_SIZE; ++d)
      {
        dir = Dir(d);
        successor.pos = current.pos + dir;
        
        if (pos_ok(successor.pos))
        {
        
          if (successor.pos == desti)
        {
          infonodes[successor.pos.i][successor.pos.j].parent = current.pos;   
          return makepath(infonodes,successor.pos);
        }
          node n1 = checkposition(successor.pos, opennodes);
          if(closednodes[successor.pos.i][successor.pos.j]) continue;
          else if( n1.f == -1 or current.f > n1.f ) {
          successor.parent = current.pos;
          successor.g = current.g + getcost(successor.pos);
          successor.h = heuristic(successor.pos, desti);
          successor.f = successor.g + successor.h;
          infonodes[successor.pos.i][successor.pos.j] = successor;
          opennodes.insert(successor); 
         }
        }
      }
    }
  } 




  int heuristics(int xorigen, int xdesti, int yorigen, int ydesti)
  {
    return abs(xdesti - xorigen) + abs(ydesti - yorigen);
  }

  
  Pos pick_city(int x, int y)
  {  
    Pos auxiliar = cities[0];
    int aux = heuristics(cities[0].i, x, cities[0].j, y);
  
    for (unsigned int r = 1; r < cities.size(); ++r)
    {
      int ret = heuristics(cities[r].i, x, cities[r].j, y);
      if (ret < aux)
      {
        aux = ret;
        auxiliar = cities[r];
      }
    }
    return auxiliar;
  }


Pos pick_city2(){

      Pos auxiliar=cities[0];
      int aux = heuristics(35, cities[0].i, 35, cities[0].j);

      for (unsigned int r = 1; r < cities.size(); ++r)
       {
      int ret = heuristics(cities[r].i, 35, cities[r].j, 35);
      if (ret < aux)
      {
        aux = ret;
        auxiliar = cities[r];
      }
    }
    return auxiliar;
}    



  map<int, stack<Pos>> orcos;

 
  void move(int id)
  {
    Unit u = unit(id);
    Pos pos = u.pos;
    Dir dir = LEFT;
    Cell c = cell(pos);
    auto it = orcos.find(id);

    
    if(round() == 1){
      Pos prova = pick_city2();
      orcos.insert(make_pair(id, search2(pos, prova)));
    }


    else if(c.type == CITY and city_owner(c.city_id) != me()){
        if(cell(pos+RIGHT).type == CITY) dir = RIGHT;
        else if(cell(pos+TOP).type == CITY) dir = TOP;
        else if(cell(pos+BOTTOM).type == CITY) dir = BOTTOM;
        else return;
    }
  

    else if(it == orcos.end()){
      Pos prova = pick_city(pos.i, pos.j);
      orcos.insert(make_pair(id,search2(pos,prova)));
      dir = dime_dir(pos,orcos[id].top());
      orcos[id].pop();
       if(orcos[id].size() == 0) orcos.erase(it);
    }

    

    
    else if(it->second.empty()) {
            Pos prova = pick_city(pos.i,pos.j);
            orcos.insert(make_pair(id, search2(pos, prova)));
            dir = dime_dir(pos,orcos[id].top());
            orcos[id].pop();
            if(orcos[id].size() == 0) orcos.erase(it);
    }


    else if(! (it->second).empty()){
          dir = dime_dir(pos,orcos[id].top());
          orcos[id].pop();
          if(orcos[id].size() == 0) orcos.erase(it);
    }
    //posibilitat quan xoques amb algú altre  
    
    Pos npos = pos + dir;

    if (pos_ok(npos))
    {
      execute(Command(id, dir));
    }
    return;
}
    
    
    /*

     /*
 
          Pos prova = pick_city((pos+dir).i, (pos+dir).j);
      orcos.insert(make_pair(id,search2(pos+dir,prova)));
      }
      else if(unit(cell(pos+RIGHT).unit_id).player!= -1 and unit(cell(pos+RIGHT).unit_id).health > u.health){
          dir = RIGHT;
     
          Pos prova = pick_city((pos+dir).i, (pos+dir).j);
      orcos.insert(make_pair(id,search2(pos+dir,prova)));
      }
      else if(unit(cell(pos+TOP).unit_id).player!= -1 and unit(cell(pos+RIGHT).unit_id).health > u.health){
          dir = TOP;


          Pos prova = pick_city((pos+dir).i, (pos+dir).j);
      orcos.insert(make_pair(id,search2(pos+dir,prova)));
      }
      else if(unit(cell(pos+BOTTOM).unit_id).player!= -1 and unit(cell(pos+RIGHT).unit_id).health > u.health){
          dir = BOTTOM;

          Pos prova = pick_city((pos+dir).i, (pos+dir).j);
      orcos.insert(make_pair(id,search2(pos+dir,prova)));
      }


    Unit u = unit(id);
    Pos pos = u.pos;

    Dir dir;
    Cell c = cell(pos);
    auto it = orcos.find(id);

    if(c.type == CITY and city_owner(c.city_id) != me())  return;


    if(c.type == CITY and round()==50 or round() == 150){
        
          Pos city = pick_city(pos.i, pos.j);
          orcos.insert(make_pair(id, search2(pos, city)));     
    }

    if (it == orcos.end()){

      Pos eficient = pick_city(pos.i,pos.j);
     
      orcos.insert(make_pair(id,search(pos,eficient)));
              
    }

    else{
      if(! (it->second).empty()){
        dir = dime_dir(pos,orcos[id].top());

        int u2 = cell(pos+dir).unit_id;

        if(unit(u2).player != me()) {
          orcos[id].pop();
        } 
        else if(u2 != -1 and unit(u2).player == me()){
          orcos.erase(it);
          Pos eficient = pick_city(pos.i,pos.j);
          orcos.insert(make_pair(id,search(pos,eficient)));
          return;
        }
        else if(unit(u2).player!= -1 and unit(u2).health > u.health){
          orcos[id].pop();
        }
        else if(unit(u2).player!= -1 and unit(u2).health < u.health){
          orcos.erase(it);
          Pos eficient = pick_city2(pos.i,pos.j);
          orcos.insert(make_pair(id,search(pos,eficient)));
          return;
        }

        if(orcos[id].size() == 0) orcos.erase(it);
        }


          orcos[id].pop();
          if(orcos[id].size() == 0) orcos.erase(it);}

        else {dir = RIGHT;}
        }



      Pos npos = pos + dir; 
      if(pos_ok(npos)){
        execute(Command(id, dir));
      } return;
    
  }
*/
  virtual void play()
  {
    vector<Pos> city;
      for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j){
          if (cell(i, j).type == CITY and city_owner(cell(i,j).city_id) != me() or cell(i, j).type == PATH and path_owner(cell(i,j).path_id) != me() )  
          {
            Pos p;
            p.i = i;
            p.j = j;
            city.push_back(p);
          }
        }
      }
      cities = city;
    
      VI my_orks = orks(me()); 

      for (int k = 0; k < int(my_orks.size()); ++k)
        move(my_orks[k]);
    
  }
};

RegisterPlayer(PLAYER_NAME);
 // pensar què passa quan xoquen i es queden quiets
  //puc fer que un cop arribem a una ciutat comencen a fer el segon tipus de search però un es queda borejant-la tot el rato (protegint-la) fent randoms dintre la ciutat
  //puc saber si estic per la meitat de les rondes amb això 
  /*
      // If more than halfway through, do nothing.
        if (round() > nb_rounds()/2) return;
  */
  
  //pensar si fer algu per matar als altres 
