#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <climits>
class str;
class node;
class edge;
class graph;
class str{
private:
    char* content;
public:
    str(const char* other = nullptr){
        if(other==nullptr)
            content= nullptr;
        else {
            content = new char[strlen(other) + 1];
            strcpy(content, other);
        }
    }
    str(const str& other){
        if(other.content == nullptr){
            delete[] content;
            content= nullptr;
        }
        else{
            delete[] content;
            content = new char[strlen(other.content)+1];
            strcpy(content,other.content);
        }
    }
    void rep(const char* other){
        delete[] content;
        content = new char[strlen(other)+1];
        strcpy(content,other);
    }
    char* rep() const{
        return content;
    }
    bool operator> (const str& a){
        if( strcmp(content, a.rep()) == -1)
            return false;
        else
            return true;
    }
    bool operator< (const str& a){
        if( strcmp(content, a.rep()) == -1)
            return true;
        else
            return false;
    }
};
class node{
public:
    str _name;
    std::vector<edge*> _node_edges;
public:
    explicit node(const char* content){
        _name.rep(content);
    }
    void add_edge(edge* content){
        _node_edges.push_back(content);
    }
};
class edge{
public:
    node* _start;
    node* _end;
    int _dist;
public:
    edge(node* start, node* end, int dist){
        _start=start;
        _end=end;
        _dist=dist;
    }
};
class graph{
public:
    int i{}, j{};
    int matrix[100][100]{};
    std::vector<edge *> _edges;
    std::vector<node *> start_node;
    std::vector<node *> end_node;
public:
    void print() {
        int len;
        int length = _edges.size();
        for (i = 0; i < length; i++) {
            std::cout<<std::endl;
            len=strlen(_edges[i]->_start->_name.rep())+strlen(_edges[i]->_end->_name.rep());
            std::cout <<i<< ". Flight from " << _edges[i]->_start->_name.rep() << " to "
                      << _edges[i]->_end->_name.rep()<<std::setw(55-len) << " Traveling time: " << (_edges[i]->_dist)/60<<" hours and "<<(_edges[i]->_dist)%60 <<" minutes." << std::endl;
        }
    }
    void add_edge(str start, str end, int dist){
        int pos_start = search(start, start_node);
        int pos_end = search(end, end_node);
        if (pos_start != -1){
            if (pos_end == -1){
                node *end_point = new node(end.rep());
                edge *new_edge = new edge(start_node[pos_start], end_point, dist);
                start_node[pos_start]->add_edge(new_edge);
                _edges.push_back(new_edge);
                end_node.push_back(end_point);
            }
            else {
                edge *new_edge = new edge(start_node[pos_start], end_node[pos_end], dist);
                start_node[pos_start]->add_edge(new_edge);
                _edges.push_back(new_edge);
            }
        }
        else {
            if (pos_end == -1){
                node *start_point = new node(start.rep());
                node *end_point = new node(end.rep());
                edge *new_edge = new edge(start_point, end_point, dist);
                start_point->add_edge(new_edge);
                _edges.push_back(new_edge);
                start_node.push_back(start_point);
                end_node.push_back(end_point);
            }
            else{
                node *start_point = new node(start.rep());
                edge *new_edge = new edge(start_point, end_node[pos_end], dist);
                start_point->add_edge(new_edge);
                _edges.push_back(new_edge);
                start_node.push_back(start_point);
            }
        }
    }
    void dijkstra(str src_1, str dest_1){
        matrix0();
        road_matrix();
        int src=search(src_1,start_node);
        if(src==-1)
        {std::cout<<"We don't have any flights departing from that airport"<<std::endl;
            return;}
        int dest=search(dest_1,start_node);
        if(dest==-1)
        {std::cout<<"We don't have any flights arriving to that airport"<<std::endl;
            return;}
        int V = start_node.size();
        int dist[V];
        bool sptSet[V];
        int parent[V];
        for (i = 0; i < V; i++) {
            parent[src] = -1;
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[src] = 0;
        for (int count = 0; count < V - 1; count++){
            int u = shortest(dist, sptSet);
            sptSet[u] = true;
            for (int v = 0; v < V; v++)
                if (!sptSet[v] && matrix[u][v] && dist[u] + matrix[u][v] < dist[v]){
                    parent[v] = u;
                    dist[v] = dist[u] + matrix[u][v];
                }
        }
        printRoute(dist, parent,src,dest);
    }
    void dijkstra2(str src_1, str dest_1){
        matrix0();
        road_matrix();
        int src=search(src_1,start_node);
        if(src==-1)
        {std::cout<<"We don't have any flights departing from that airport"<<std::endl;
            return;}
        int dest=search(dest_1,start_node);
        if(dest==-1)
        {std::cout<<"We don't have any flights arriving to that airport"<<std::endl;
            return;}
        int V = start_node.size();
        int dist[V];
        bool sptSet[V];
        int parent[V];
        for (i = 0; i < V; i++) {
            parent[src] = -1;
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[src] = 0;
        for (int count = 0; count < V - 1; count++){
            int u = shortest(dist, sptSet);
            sptSet[u] = true;
            for (int v = 0; v < V; v++)
                if (!sptSet[v] && matrix[u][v] && dist[u] + matrix[u][v] < dist[v]){
                    parent[v] = u;
                    dist[v] = dist[u] + matrix[u][v];
                }
        }
        printRoute2(dist, parent,src,dest);
    }
    void printAirports() const{
        std::cout << "Airports: " << std::endl;
        show_list(start_node);
    }
    void dijkstra_all(str src_1){
        matrix0();
        road_matrix();
        int src = search(src_1,start_node);
        if(src==-1)
        {std::cout<<"We don't have any flights departing from that airport"<<std::endl;
            return;}
        int V = start_node.size();
        int dist[V];
        bool sptSet[V];
        int parent[V];
        for (i = 0; i < V; i++) {
            parent[src] = -1;
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[src] = 0;
        for (int count = 0; count < V - 1; count++) {
            int u = shortest_all(dist, sptSet);
            sptSet[u] = true;
            for (int v = 0; v < V; v++)
                if (!sptSet[v] && matrix[u][v] && dist[u] + matrix[u][v] < dist[v]) {
                    parent[v] = u;
                    dist[v] = dist[u] + matrix[u][v];
                }
        }
        print_all_routes(dist, parent,src);
    }
private:
    static void show_list(std::vector<node *> a) {
        int length = a.size();
        for (int i = 0; i < length; i++)
            std::cout <<i+1<<". "<< a[i]->_name.rep() << std::endl;
        std::cout << std::endl;
    }
    int search(str &name, std::vector<node *> other){
        int length = other.size();
        int pos = -1;
        for (i = 0; i < length; i++)
            if (strcmp(name.rep(), other[i]->_name.rep()) == 0)
                pos = i;
        return pos;
    }
    void matrix0() {
        int length = start_node.size();
        for (i = 0; i < length; i++)
            for (j = 0; j < length; j++)
                matrix[i][j] = 0;
    }
    void road_matrix(){
        int edge_length = _edges.size();
        int k;
        for (k = 0; k < edge_length; k++) {
            str temp = _edges[k]->_start->_name.rep();
            str temp2 = _edges[k]->_end->_name.rep();
            int x = search(temp, start_node);
            int y = search(temp2, start_node);
            if (x != -1 && y != -1)
                matrix[x][y] = _edges[k]->_dist;
        }
    }
    int shortest(const int dist[], const bool sptSet[]) const{
        int V = start_node.size();
        int min = INT_MAX, min_index;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && dist[v] <= min)
                min = dist[v], min_index = v;
        return min_index;
    }
    void printRoad(int parent[], int p) {
        if (parent[p] == -1)
            return;
        printRoad(parent, parent[p]);
        std::cout<<" -> "<<start_node[p]->_name.rep();
    }
    void printRoute(int dist[], int parent[],int src,int dest) {
        std::cout<<std::endl;
        std::cout<<"Trip:      "<<start_node[src]->_name.rep()<<" -> "<<start_node[dest]->_name.rep()<<std::endl;
        std::cout<<"Distance:  "<<dist[dest]/60<<" hours and "<<dist[dest]%60<<" minutes."<<std::endl;
        std::cout<<"Route:     "<<start_node[src]->_name.rep()<<" ";
        printRoad(parent,dest);
    }
    void printRoute2(int dist[], int parent[],int src,int dest) {
        std::cout<<std::endl;
        std::cout<<"Trip:      "<<start_node[src]->_name.rep()<<" -> "<<start_node[dest]->_name.rep()<<std::endl;
        std::cout<<"Price:     "<<dist[dest]<<" $"<<std::endl;
        std::cout<<"Route:     "<<start_node[src]->_name.rep()<<" ";
        printRoad(parent,dest);
    }
    int shortest_all(const int dist[], const bool sptSet[]) const {
        int V = start_node.size();
        int min = INT_MAX, min_index;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && dist[v] <= min)
                min = dist[v], min_index = v;
        return min_index;
    }
    void printRoad_all(int parent[], int p) {
        if (parent[p] == -1)
            return;
        printRoad_all(parent, parent[p]);
        std::cout<<" -> "<<start_node[p]->_name.rep();
    }
    void print_all_routes(int dist[], int parent[],int src) {
        int V = start_node.size();
        int len;
        int len2=30;
        std::cout<<"Starting airport:     "<<std::setw(21)<<"Time:"<<std::setw(38-strlen(start_node[src]->_name.rep()))<<"Path:"<<std::endl;
        for (i = 0; i < V; i++) {
            len=35-strlen(start_node[src]->_name.rep())-strlen(start_node[i]->_name.rep());
            if(strcmp(start_node[src]->_name.rep(),start_node[i]->_name.rep())!=0){
                if((dist[i]%60)/10!=0)
                {std::cout<<std::endl<<start_node[src]->_name.rep()<<" -> "<<start_node[i]->_name.rep()<<std::setw(len)<<dist[i]/60<<" hours and "<<dist[i]%60<<" minutes"<<std::setw(16)<<start_node[src]->_name.rep()<<" ";
                    printRoad_all(parent, i);}
                else
                {std::cout<<std::endl<<start_node[src]->_name.rep()<<" -> "<<start_node[i]->_name.rep()<<std::setw(len)<<dist[i]/60<<" hours and "<<dist[i]%60<<" minutes "<<std::setw(16)<<start_node[src]->_name.rep()<<" ";
                    printRoad_all(parent, i);}
            }

        }

    }

};

void menu(){
    std::cout << "1. Take a look at the airports. " << std::endl;
    std::cout << "2. Find the fastest route. " << std::endl;
    std::cout << "3. Find the cheapest route." << std::endl;
    std::cout << "4. See all routes from an airport." << std::endl;
    std::cout << "5. Check all the direct flights." << std::endl;
    std::cout << "9. Exit." << std::endl;
}
void main_menu(graph& T,graph& T2){
    int choice = 0;
    menu();

    char src_1[50];
    char src_2[50];
    char dest_1[50];
    std::cin >> choice;
    switch (choice) {
        case 1:
            T.printAirports();
            main_menu(T,T2);
            break;
        case 2:
            std::cout<<"Please enter the source airport:";
            std::cin>>src_1;
            std::cout<<"Please enter the destination airport:";
            std::cin>>dest_1;
            T.dijkstra(src_1,dest_1);
            std::cout<<std::endl;
            std::cout<<std::endl;
            main_menu(T,T2);
            break;
        case 3:
            std::cout<<"Please enter the source airport:";
            std::cin>>src_1;
            std::cout<<"Please enter the destination airport:";
            std::cin>>dest_1;
            T2.dijkstra2(src_1,dest_1);
            std::cout<<std::endl;
            std::cout<<std::endl;
            main_menu(T,T2);
            break;
        case 4:
            T.print();
            std::cout<<std::endl;
            main_menu(T,T2);
            break;
        case 5:
            std::cout<<"Please enter the source airport:";
            std::cin>>(src_2);
            T.dijkstra_all(src_2);
            std::cout<<std::endl;
            std::cout<<std::endl;
            main_menu(T,T2);
            break;
        case 9:
            std::cout<<"Goodbye! Have a good flight!";
            break;
        default:
            std::cout<<"Invalid choice";
            std::cout<<std::endl;
            break;

    }
}

void add_nodes(graph& T){
    //Singapore ->
    T.add_edge("Singapore","Jakarta",105);
    T.add_edge("Singapore","San-Francisco",1105);
    T.add_edge("Singapore","Manila",220);
    T.add_edge("Singapore","Sydney",465);
    //Jakarta ->
    T.add_edge("Jakarta","Singapore",105);
    T.add_edge("Jakarta","Tokyo",450);
    T.add_edge("Jakarta","Abu-Dhabi",480);
    //San Francisco ->
    T.add_edge("San-Francisco","Singapore",1105);
    T.add_edge("San-Francisco","Atlanta",280);
    T.add_edge("San-Francisco","Mexico-City",270);
    //Manila ->
    T.add_edge("Manila","Singapore",220);
    T.add_edge("Manila","Jakarta",250);
    T.add_edge("Manila","Istanbul",1185);
    //Tokyo ->
    T.add_edge("Tokyo","Jakarta",450);
    T.add_edge("Tokyo","Munich",1100);
    T.add_edge("Tokyo","Hong-Kong",295);
    //Abu Dhabi ->
    T.add_edge("Abu-Dhabi","Jakarta",480);
    T.add_edge("Abu-Dhabi","London",580);
    T.add_edge("Abu-Dhabi","Istanbul",290);
    //Atlanta ->
    T.add_edge("Atlanta","San-Francisco",280);
    T.add_edge("Atlanta","Chicago",120);
    T.add_edge("Atlanta","Las-Vegas",255);
    T.add_edge("Atlanta","Los-Angeles",285);
    T.add_edge("Atlanta","New-York-City",135);
    //Mexico City ->
    T.add_edge("Mexico-City","San-Francisco",280);
    T.add_edge("Mexico-City","Bogota",270);
    T.add_edge("Mexico-City","Buenos-Aires",745);
    //Istanbul ->
    T.add_edge("Istanbul","Lisbon",300);
    T.add_edge("Istanbul","Rome",170);
    T.add_edge("Istanbul","Paris",225);
    //Munich ->
    T.add_edge("Munich","Tokyo",1100);
    T.add_edge("Munich","Rome",90);
    T.add_edge("Munich","Amsterdam",95);
    //Hong Kong ->
    T.add_edge("Hong-Kong","Manila",130);
    T.add_edge("Hong-Kong","Singapore",215);
    T.add_edge("Hong-Kong","Istanbul",675);
    //London ->
    T.add_edge("London","Amsterdam",70);
    T.add_edge("London","Paris",80);
    T.add_edge("London","Rome",140);
    //Chicago ->
    T.add_edge("Chicago","Los-Angeles",260);
    T.add_edge("Chicago","Las-Vegas",225);
    T.add_edge("Chicago","Atlanta",105);
    //Las Vegas ->
    T.add_edge("Las-Vegas","Atlanta",225);
    T.add_edge("Las-Vegas","Mexico-City",230);
    T.add_edge("Las-Vegas","New-York-City",280);
    //Los Angeles->
    T.add_edge("Los-Angeles","Mexico-City",215);
    T.add_edge("Los-Angeles","Atlanta",260);
    T.add_edge("Los-Angeles","Chicago",215);
    //New York City ->
    T.add_edge("New-York-City","London",670);
    T.add_edge("New-York-City","Atlanta",135);
    T.add_edge("New-York-City","Paris",685);
    //Bogota ->
    T.add_edge("Bogota","Buenos-Aires",370);
    T.add_edge("Bogota","Mexico-City",275);
    T.add_edge("Bogota","Rio-de-Janeiro",600);
    //Buenos Aires ->
    T.add_edge("Buenos-Aires","Bogota",370);
    T.add_edge("Buenos-Aires","Rio-de-Janeiro",175);
    T.add_edge("Buenos-Aires","Lima",290);
    //Lisbon ->
    T.add_edge("Lisbon","London",155);
    T.add_edge("Lisbon","Budapest",215);
    T.add_edge("Lisbon","Frankfurt",180);
    //Rome ->
    T.add_edge("Rome","Bucharest",115);
    T.add_edge("Rome","Frankfurt",115);
    T.add_edge("Rome","London",155);
    //Paris ->
    T.add_edge("Paris","London",65);
    T.add_edge("Paris","Rome",120);
    T.add_edge("Paris","Barcelona",105);
    //Amsterdam ->
    T.add_edge("Amsterdam","Lisbon",175);
    T.add_edge("Amsterdam","Barcelona",130);
    T.add_edge("Amsterdam","Ibiza",155);
    //Rio de Janeiro ->
    T.add_edge("Rio-de-Janeiro","Mexico-City",765);
    T.add_edge("Rio-de-Janeiro","Atlanta",745);
    T.add_edge("Rio-de-Janeiro","Lima",590);
    //Lima ->
    T.add_edge("Lima","Buenos-Aires",260);
    T.add_edge("Lima","Rio-de-Janeiro",590);
    T.add_edge("Lima","Mexico-city",595);
    //Budapest ->
    T.add_edge("Budapest","Frankfurt",100);
    T.add_edge("Budapest","Bucharest",105);
    T.add_edge("Budapest","London",150);
    //Bucharest ->
    T.add_edge("Bucharest","Rome",120);
    T.add_edge("Bucharest","Athens",95);
    T.add_edge("Bucharest","Barcelona",200);
    //Frankfurt ->
    T.add_edge("Frankfurt","Bucharest",100);
    T.add_edge("Frankfurt","London",100);
    T.add_edge("Frankfurt","Paris",75);
    T.add_edge("Frankfurt","Rome",110);
    T.add_edge("Frankfurt","Barcelona",120);
    T.add_edge("Frankfurt","Istanbul",190);
    //Barcelona ->
    T.add_edge("Barcelona","Lisbon",120);
    T.add_edge("Barcelona","Paris",110);
    T.add_edge("Barcelona","Munich",120);
    //Ibiza ->
    T.add_edge("Ibiza","Barcelona",55);
    T.add_edge("Ibiza","Frankfurt",145);
    //Athens ->
    T.add_edge("Athens","Bucharest",95);
    T.add_edge("Athens","Budapest",120);
    T.add_edge("Athens","Istanbul",80);
    //Sydney ->
    T.add_edge("Sydney","Singapore",495);
    T.add_edge("Sydney","Honolulu",570);
    T.add_edge("Sydney","Auckland",180);
    //Honolulu ->
    T.add_edge("Honolulu","Los-Angeles",330);
    T.add_edge("Honolulu","Atlanta",700);
    T.add_edge("Honolulu","Sydney",615);
    //Auckland ->
    T.add_edge("Auckland","Manila",1260);
    T.add_edge("Auckland","Tokyo",660);
    T.add_edge("Auckland","Singapore",950);

}
void add_nodes2(graph& T2){
    //Singapore ->
    T2.add_edge("Singapore","Jakarta",112);
    T2.add_edge("Singapore","San-Francisco",675);
    T2.add_edge("Singapore","Manila",122);
    T2.add_edge("Singapore","Sydney",215);
    //Jakarta ->
    T2.add_edge("Jakarta","Singapore",58);
    T2.add_edge("Jakarta","Tokyo",713);
    T2.add_edge("Jakarta","Abu-Dhabi",425);
    //San Francisco ->
    T2.add_edge("San-Francisco","Singapore",847);
    T2.add_edge("San-Francisco","Atlanta",259);
    T2.add_edge("San-Francisco","Mexico-City",246);
    //Manila ->
    T2.add_edge("Manila","Singapore",70);
    T2.add_edge("Manila","Jakarta",209);
    T2.add_edge("Manila","Istanbul",446);
    //Tokyo ->
    T2.add_edge("Tokyo","Jakarta",286);
    T2.add_edge("Tokyo","Munich",503);
    T2.add_edge("Tokyo","Hong-Kong",314);
    //Abu Dhabi ->
    T2.add_edge("Abu-Dhabi","Jakarta",262);
    T2.add_edge("Abu-Dhabi","London",195);
    T2.add_edge("Abu-Dhabi","Istanbul",265);
    //Atlanta ->
    T2.add_edge("Atlanta","San-Francisco",255);
    T2.add_edge("Atlanta","Chicago",107);
    T2.add_edge("Atlanta","Las-Vegas",187);
    T2.add_edge("Atlanta","Los-Angeles",299);
    T2.add_edge("Atlanta","New-York-City",122);
    //Mexico City ->
    T2.add_edge("Mexico-City","San-Francisco",192);
    T2.add_edge("Mexico-City","Bogota",121);
    T2.add_edge("Mexico-City","Buenos-Aires",391);
    //Istanbul ->
    T2.add_edge("Istanbul","Lisbon",157);
    T2.add_edge("Istanbul","Rome",91);
    T2.add_edge("Istanbul","Paris",131);
    //Munich ->
    T2.add_edge("Munich","Tokyo",1272);
    T2.add_edge("Munich","Rome",195);
    T2.add_edge("Munich","Amsterdam",175);
    //Hong Kong ->
    T2.add_edge("Hong-Kong","Manila",225);
    T2.add_edge("Hong-Kong","Singapore",156);
    T2.add_edge("Hong-Kong","Istanbul",528);
    //London ->
    T2.add_edge("London","Amsterdam",78);
    T2.add_edge("London","Paris",84);
    T2.add_edge("London","Rome",143);
    //Chicago ->
    T2.add_edge("Chicago","Los-Angeles",179);
    T2.add_edge("Chicago","Las-Vegas",196);
    T2.add_edge("Chicago","Atlanta",67);
    //Las Vegas ->
    T2.add_edge("Las-Vegas","Atlanta",136);
    T2.add_edge("Las-Vegas","Mexico-City",96);
    T2.add_edge("Las-Vegas","New-York-City",189);
    //Los Angeles->
    T2.add_edge("Los-Angeles","Mexico-City",139);
    T2.add_edge("Los-Angeles","Atlanta",121);
    T2.add_edge("Los-Angeles","Chicago",179);
    //New York City ->
    T2.add_edge("New-York-City","London",571);
    T2.add_edge("New-York-City","Atlanta",43);
    T2.add_edge("New-York-City","Paris",500);
    //Bogota ->
    T2.add_edge("Bogota","Buenos-Aires",286);
    T2.add_edge("Bogota","Mexico-City",124);
    T2.add_edge("Bogota","Rio-de-Janeiro",581);
    //Buenos Aires ->
    T2.add_edge("Buenos-Aires","Bogota",999);
    T2.add_edge("Buenos-Aires","Rio-de-Janeiro",360);
    T2.add_edge("Buenos-Aires","Lima",535);
    //Lisbon ->
    T2.add_edge("Lisbon","London",116);
    T2.add_edge("Lisbon","Budapest",93);
    T2.add_edge("Lisbon","Frankfurt",65);
    //Rome ->
    T2.add_edge("Rome","Bucharest",81);
    T2.add_edge("Rome","Frankfurt",137);
    T2.add_edge("Rome","London",122);
    //Paris ->
    T2.add_edge("Paris","London",44);
    T2.add_edge("Paris","Rome",74);
    T2.add_edge("Paris","Barcelona",52);
    //Amsterdam ->
    T2.add_edge("Amsterdam","Lisbon",188);
    T2.add_edge("Amsterdam","Barcelona",190);
    T2.add_edge("Amsterdam","Ibiza",146);
    //Rio de Janeiro ->
    T2.add_edge("Rio-de-Janeiro","Mexico-City",515);
    T2.add_edge("Rio-de-Janeiro","Atlanta",553);
    T2.add_edge("Rio-de-Janeiro","Lima",244);
    //Lima ->
    T2.add_edge("Lima","Buenos-Aires",346);
    T2.add_edge("Lima","Rio-de-Janeiro",347);
    T2.add_edge("Lima","Mexico-city",244);
    //Budapest ->
    T2.add_edge("Budapest","Frankfurt",108);
    T2.add_edge("Budapest","Bucharest",140);
    T2.add_edge("Budapest","London",92);
    //Bucharest ->
    T2.add_edge("Bucharest","Rome",23);
    T2.add_edge("Bucharest","Athens",110);
    T2.add_edge("Bucharest","Barcelona",73);
    //Frankfurt ->
    T2.add_edge("Frankfurt","Bucharest",164);
    T2.add_edge("Frankfurt","London",121);
    T2.add_edge("Frankfurt","Paris",158);
    T2.add_edge("Frankfurt","Rome",274);
    T2.add_edge("Frankfurt","Barcelona",194);
    T2.add_edge("Frankfurt","Istanbul",160);
    //Barcelona ->
    T2.add_edge("Barcelona","Lisbon",74);
    T2.add_edge("Barcelona","Paris",69);
    T2.add_edge("Barcelona","Munich",48);
    //Ibiza ->
    T2.add_edge("Ibiza","Barcelona",11);
    T2.add_edge("Ibiza","Frankfurt",43);
    //Athens ->
    T2.add_edge("Athens","Bucharest",124);
    T2.add_edge("Athens","Budapest",11);
    T2.add_edge("Athens","Istanbul",69);
    //Sydney ->
    T2.add_edge("Sydney","Singapore",189);
    T2.add_edge("Sydney","Honolulu",255);
    T2.add_edge("Sydney","Auckland",419);
    //Honolulu ->
    T2.add_edge("Honolulu","Los-Angeles",254);
    T2.add_edge("Honolulu","Atlanta",581);
    T2.add_edge("Honolulu","Sydney",549);
    //Auckland ->
    T2.add_edge("Auckland","Manila",777);
    T2.add_edge("Auckland","Tokyo",677);
    T2.add_edge("Auckland","Singapore",437);

}
int main() {
    graph T,T2;
    add_nodes(T);
    add_nodes2(T2);
    main_menu(T,T2);
    return 0;
}

