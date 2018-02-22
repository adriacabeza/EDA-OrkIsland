#include "Player.hh"
#include <algorithm>
#include <stack> 
#include <queue>
#include <vector>
#include <cmath>
#include <map>



#define PLAYER_NAME Tupac



struct PLAYER_NAME : public Player
{

  static Player *factory()
  {
    return new PLAYER_NAME;
  }

  //VARIABLES GLOBALES

  typedef vector<int> VI;
  vector<Pos> cities; 
  map<int, vector<Pos> > orco;


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //ESTRUCTURES


  struct node
  {
    Pos pos;
    Pos parent;
    int g; // suma del cost de la cel·la actual amb la del successor
    int h; //distància des del destí al successor
    int f; //suma de la distància  i del cost des del destí al successor

    friend bool operator<(node left, node right)
    {
      return (left.f < right.f);
    }
  };


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //FUNCIONS


//it returns on which city I should go (the nearest)
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


  //calcul de costos segons el tipus de cel·la
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
      return 800000000000;
    else if (c.type == FOREST)
      return 400000;
    return 0;
  }



  int heuristics(int xorigen, int xdesti, int yorigen, int ydesti)
  {
    return abs(xdesti - xorigen) + abs(ydesti - yorigen);
  }


 //Retorna la diferència de posició entesa com a una heurística Manhattan de diferència d'eixos.
  int heuristic(const Pos& origen,const  Pos& desti){
    return abs(origen.i - desti.i) + abs(origen.j - desti.j);
  }

  //Comprova si una posició a està al conjunt b.
node checkposition(const Pos& a, const set<node>& b)
  {
    for (auto it = b.begin(); it != b.end(); ++it)
    {
      if ((*it).pos == a)
        return *it;
    }
    node c;
    c.f = -1;
    return c;
  }


  //Retorna la direcció a la que ha d'anar origen per a arribar al destí.
Dir dime_dir(const Pos& origen, const Pos& desti)
  {
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

  //Donat un array de nodes(on estan situats els visitats amb la seva info) i un destí i mitjançant
  //la posició pare de l'struct es traça el camí ideal per al meu orco en forma de pila de posicions

stack <Pos> makepath(node infonodes[70][70], const Pos& desti){

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

 //Fill de la primera versió del search, aquesta versió té en compte els
  //camins i les ciutats com a coses MOLT més bones que els altres tipus de
  //cel·les. He variat el tipus de cost per al meu benefici


stack<Pos> search2(const Pos& inicio,const Pos& desti)
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




 //Segona versió del pickcity. Aquesta versió no et retorna la ciutat que està  més aprop
 //de l'orco sinó que et retorna la ciutat que està més aprop del centre


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


 //funció cridada a cada ronda, desde la funció es fa tota la jugada.
  //si no l'he vist cap cop o ja ha arribat al destí otorgat a la primera
  //ronda se li assigna un nou amb un altre camí diferent

 
  void move(int id)
  {
    Unit u = unit(id);
    Pos pos = u.pos;
    Dir dir = LEFT;
    Cell c = cell(pos);
    auto it = orcos.find(id);

    
    if(round() == 0){
      Pos prova = pick_city2();
      orcos.insert(make_pair(id, search2(pos, prova)));
      dir = dime_dir(pos,orcos[id].top());
      orcos[id].pop();
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
    
    Pos npos = pos + dir;

    if (pos_ok(npos))
    {
      execute(Command(id, dir));
    }
    return;
}
    
    
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
