#include <iostream>
#include <stdlib.h>
#include <time.h>  
#include <bits/stdc++.h>
 
using namespace std;

bool isValid(int x, int y){ return (x<8 && x>=0 && y<8 && y>=0);}
class Space;
class Piece {
  public:
    virtual int getType() = 0;
    virtual bool getTeam() = 0;
    virtual vector<string> getMoves(Space** board, int curX, int cuY) = 0;
};
class Man : public Piece {
    bool team;
  public:
    Man(bool team) : team(team){}
    int getType() { return 1;}
    bool getTeam() { return team;}
    vector<string> getMoves(Space** board, int curX, int curY);
};
class King : public Piece {
    bool team;
  public:
    King(bool team): team(team) {}
    int getType() { return 2;}
    bool getTeam() { return team;}
    vector<string> getMoves(Space** board, int curX, int curY);
};
class Space {
    Piece* ptr=nullptr;
  public:
    void setPtrEmpty(Piece* p) {ptr=nullptr;}
    void setPtr(Man *m) { ptr=m;}
    void setPtr(King *k) { ptr=k;}
    Piece* getPtr(){return ptr;}
};
class Board {
    bool whoToMove;
    Space **board;
  private:
    void makeMove(int x0, int y0, int x1, int y1){
        if(x1==0 || x1==7) {
            bool team= (board[x0][y0].getPtr())->getTeam();
            delete (board[x0][y0].getPtr());
            board[x0][y0].setPtr(new King(team));
        }
        swap(board[x1][y1],board[x0][y0]);
        if(((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0)) > 2){
            if(board[x1-(x1>x0 ? 1 : -1)][y1-(y1>y0 ? 1 : -1)].getPtr()){
                delete (board[x1-(x1>x0 ? 1 : -1)][y1-(y1>y0 ? 1 : -1)].getPtr());
                board[x1-(x1>x0 ? 1 : -1)][y1-(y1>y0 ? 1 : -1)].setPtrEmpty(nullptr);
            }
        }
        whoToMove^=true;
    }
    void moveComputer(bool tt=1){
        vector<long long> bM; vector<long long> nM;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                Piece *ptr=board[i][j].getPtr();
                if(!ptr) continue;
                if(ptr->getTeam()==tt) continue;
                vector<string> toAdd = getMoves(j, i);
                for(int k=0;k<toAdd.size();k++){
                    int xs=toAdd[k][1]-'1', ys=toAdd[k][0]-'A';
                    int d=i+8*j+64*xs+512*ys;
                    if((xs-i)*(xs-i)+(ys-j)*(ys-j)>2){
                        // cout << (char) (j+'A') << (char) (i+'1') << "->" << (char) (ys-(ys>j ? 1 : -1)+'A') << (char) (xs-(xs>i ? 1 : -1)+'1') << " ";
                        // cout << ys-(ys>j ? 1 : -1) << " " << xs-(xs>i ? 1 : -1) << board[ys-(ys>j ? 1 : -1)][xs-(xs>i ? 1 : -1)].getPtr() << "  ";
                        if(board[xs-(xs>i ? 1 : -1)][ys-(ys>j? 1 : -1)].getPtr()){
                            // cout << tt << " " << (board[xs-(xs>i ? 1 : -1)][ys-(ys>j ? 1 : -1)].getPtr())->getTeam() << "           ";
                            if((board[xs-(xs>i ? 1 : -1)][ys-(ys>j ? 1 : -1)].getPtr())->getTeam()==tt)
                                bM.push_back(d);
                            else
                                nM.push_back(d);
                        } else nM.push_back(d);
                    } else nM.push_back(d);
                }
            }
        }
        // cout << "Total moves for " << (tt? "green" : "red") << " good: " << (bM.size()) << "   bad: " << (nM.size()) << (char) 10;
        // cout << "good: ";for(int i=0;i<bM.size();i++) cout << (char)((bM[i]/8)%8+'A') << (char)((bM[i]/1)%8+'1') << "->"<< (char)((bM[i]/512)%8+'A') << (char)((bM[i]/64)%8+'1') << "   ";
        // cout << (char) 10 << "bad: ";for(int i=0;i<nM.size();i++) cout << (char)((nM[i]/8)%8+'A') << (char)((nM[i]/1)%8+'1') << "->"<< (char)((nM[i]/512)%8+'A') << (char)((nM[i]/64)%8+'1') << "   ";
        if(bM.size()==0 && nM.size()==0) return;
        if(bM.size()) {
            int r=rand()%bM.size();
            // cout << (char)((bM[r]/8)%8+'A') << (char)((bM[r]/1)%8+'1') << (char)((bM[r]/512)%8+'A') << (char)((bM[r]/64)%8+'1');
            makeMove((bM[r]/1)%8,(bM[r]/8)%8, (bM[r]/64)%8, (bM[r]/512)%8);
        } else {
            int r=rand()%nM.size();
            // cout << (char)((nM[r]/8)%8+'A') << (char)((nM[r]/1)%8+'1') << " " << (char)((nM[r]/512)%8+'A') << (char)((nM[r]/64)%8+'1');
            makeMove((nM[r]/1)%8,(nM[r]/8)%8, (nM[r]/64)%8, (nM[r]/512)%8);
        }
    }
  public:
    void PlayAgainstItself(){
        bool turn=false;
        while(!checkEnd()){
            moveComputer(turn);
            printBoard();
            turn^=true;
            // for(long long i=0;i<3e9+5;i++){}
        }
        cout << "Player " << checkEnd() << " wins" << (char) 10;
    }
    Board() : whoToMove(0) {
        board = new Space*[8];
        for(int i=0;i<8;i++){
            board[i] = new Space[8];
        }
        // board[2][2].setPtr(new Man(true));
        for(int i=0;i<3;i++){
            for(int j=(i&1); j<8; j+=2)
                board[i][j].setPtr(new Man(true));
            for(int j=!(i&1); j<8; j+=2)
                board[7-i][j].setPtr(new Man(false));
        }
    }
    bool isEmpty(int x, int y){ return (board[x][y].getPtr() ? 0 : 1);}
    vector<string> getMoves(int x, int y){vector<string> s;Piece* p=board[y][x].getPtr(); if(p) return p->getMoves(board, x, y); else return s;}
    void printBoard(){
        cout << (char) 10;
        for(int oo=0;oo<33;oo++) 
            cout << (oo==32? "\u2513" : (oo==0 ? "\u250F" : (oo%4==0? "\u2533" : "\u2501")));
        for(int i=7;i>=0;i--){
            cout << (char) 10 << "\u2503" << " ";
            for(int j=0;j<8;j++){
                Piece* p=board[i][j].getPtr();
                if(p) cout << (p->getType() == 1? (p->getTeam() ? "\033[1;31mP\033[0m" : "\033[1;32mP\033[0m") : (p->getTeam() ? "\033[1;31mK\033[0m" : "\033[1;32mK\033[0m")) << " " << "\u2503" << " ";
                else cout << "  " << "\u2503" << " ";
            }
            cout << "\033[1;36m" << (char) (i+'1') << "\033[0m";
            cout << (char) 10; 
            for(int oo=0;oo<33;oo++) {
                if(!i) cout << (oo==32? "\u251B" : (oo==0 ? "\u2517" : (oo%4==0? "\u253B" : "\u2501")));
                else cout << (oo==32? "\u252B" : (oo==0 ? "\u2523" : (oo%4==0? "\u254B" : "\u2501")));
            }
        }
        cout << (char) 10 << "\033[1;36m";
        for(int oo=0;oo<33;oo++) {
            if(oo%4==2) cout << (char) (oo/4+'A');
            else cout << " ";
        }
        cout << (char) 10 << (char) 10<< "\033[0m";
    }
    void movePlayer(){
        while(1){
            cout << "\033[1;37m";
            cout << "Digite a peca que quer mover (Ex: A3): ";
            string p;
            cin >> p;
            cout << "Peca escolhida: " << p << (char) 10;
            if(p.size()!=2)
                continue;
            if(p[0]-'A'>7 || p[0]-'A'<0 || p[1]-'1'>7 || p[1]-'1'<0){
                cout << "Lugar invalido" << (char) 10;
                continue;
            }
            int x=p[0]-'A', y=p[1]-'1';
            if(!board[y][x].getPtr()){
                cout << "Nao ha peca nesse lugar" << (char) 10;
                continue;
            }
            if(!(board[y][x].getPtr())->getTeam()) {
                cout << "Essa peca nao eh sua." << (char) 10;
                continue;
            }
            vector<string> moves=getMoves(x,y);
            if(!moves.size()) {
                cout << "Nao existem movimentos validos para essa peca no momento." << (char) 10;
                continue;
            }
            cout << (char) 10 << "Movimentos validos para essa peca: ";
            for(int i=0;i<moves.size();i++) cout << p << "->" << moves[i] << " ";
            bool cont=true;
            while(cont){
                cout << (char) 10 << "Destino da peca (digite X para escolher outra peca): ";
                string to;
                cin >> to;
                if(to=="X") {cont=true;break;}
                for(int i=0;i<moves.size();i++){
                    if(to==moves[i]){
                        makeMove(p[1]-'1', p[0]-'A', moves[i][1]-'1', moves[i][0]-'A');
                        cout << "\033[0m";
                        printBoard();
                        if(checkEnd()) {
                            cout << "Player " << checkEnd() << " wins" << (char) 10;
                            return;
                        }
                        cout << (char) 10 << "Vez do computador, pensando..." << (char) 10;
                        moveComputer();
                        cont=false;
                        return;
                    }
                    if(i==moves.size()-1)
                        cout << "Movimento nao disponivel" << (char) 10;
                }
            }
        }
    }
    int checkEnd(){
        bool a=false, b=false;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(board[i][j].getPtr()){
                    if((board[i][j].getPtr())->getTeam()) a=true;
                    else b=true;
                }
                if(a&b) return 0;
            }
        }
        if(a & !b) return 1;
        if(!a & b) return 2;
        return 0;
    }
};

vector<string> Man::getMoves(Space** board, int curX, int curY){
    vector<string> ans;
    int dy= (team? 1 : -1);
    int dx[2]={1,-1};
    for(int i=0;i<2;i++){
        if(isValid(curX+dx[i], curY+dy)){
            if(!(board[curY+dy][curX+dx[i]].getPtr())){
                // cout << "trying : " << curX+dx[i] << " " << curY+dy << (char) 10;
                string tmp="";tmp=(char) (curX+dx[i]+'A'); tmp+=(char) (curY+dy+'1');
                ans.push_back(tmp);
            }
        }
    }
    for(int i=0;i<2;i++){
        if(isValid(curX+2*dx[i], curY+2*dy)){
            Piece* ptr=board[curY+dy][curX+dx[i]].getPtr();
            if(ptr && !(board[curY+2*dy][curX+2*dx[i]].getPtr())){
                if(ptr->getTeam()!=team){
                    string s="";s+=(char) (curX+2*dx[i]+'A'); s+=(char) (curY+2*dy+'1');
                    ans.push_back(s);
                }
            }
        }
    }
    return ans;
}

vector<string> King::getMoves(Space** board, int curX, int curY){
    vector<string> ans;
    string tmp="";
    int dy[4]={-1, 1, 1, -1};
    int dx[4]={1,-1, 1, -1};
    for(int i=0;i<4;i++){
        if(isValid(curX+dx[i], curY+dy[i])){
            if(!(board[curY+dy[i]][curX+dx[i]].getPtr())){
                // cout << "trying : " << curX+dx[i] << " " << curY+dy[i] << (char) 10;
                tmp=(char) (curX+dx[i]+'A'); tmp+=(char) (curY+dy[i]+'1');
                ans.push_back(tmp); tmp="";
            }
        }
    }
    for(int i=0;i<4;i++){
        int l=1;
        while(isValid(curX+(1+l)*dx[i], curY+(1+l)*dy[i])){
            Piece* ptr=board[curY+l*dy[i]][curX+l*dx[i]].getPtr(), *ptr2=board[curY+(1+l)*dy[i]][curX+(1+l)*dx[i]].getPtr();
            if(ptr && ptr2) break;
            if(ptr && !ptr2){
                if(ptr->getTeam()!=team){
                    string s="";s+=(char) (curX+(1+l)*dx[i]+'A'); s+=(char) (curY+(1+l)*dy[i]+'1');
                    ans.push_back(s);
                    break;
                }
            } else if (ptr2){
                if(ptr2->getTeam()==team)
                    break;
            } else if(!ptr2) {
                string s="";s+=(char) (curX+(1+l)*dx[i]+'A'); s+=(char) (curY+(1+l)*dy[i]+'1');
                ans.push_back(s);
            }
            l++;
        }
    }
    return ans;
}


int main(){
    Board b;
    srand (time(NULL));
    b.PlayAgainstItself();
    // while(1){
    //     b.printBoard();
    //     b.movePlayer();
    //     if(b.checkEnd()) {
    //         cout << "Player " << b.checkEnd() << " wins" << (char) 10;
    //         break;
    //     }
    // }
    return 0;
}