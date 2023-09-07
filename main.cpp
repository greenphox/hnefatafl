#include <bits/stdc++.h>
using namespace std;

int depth ;

class Status
{
public:
    array<short, 49> board;
    int king_pos;
    int winner = 0;
    int whitepieces = 4;
    int blackpieces = 8;
    int interest_value=0;
    bool capture = false; // was there a capture in this turn?

    Status(array<short, 49> boarda){
        board=boarda;
        whitepieces=0;
        blackpieces=0;
        for(int i=0; i<49; i++){
            if (board[i]==3)
                king_pos=i;
            else if (board[i]==2)
                whitepieces++;
            else if (board[i]==2)
                blackpieces++;
        }
    }
};

struct CompareMoves {
    bool operator()(const Status& a, const Status& b) const {
        return a.interest_value > b.interest_value; // Change to < for min-heap behavior
    }
};

// EDITS the board sent according to the moves.
void MaterializeMove(Status *status, int startp, int endp)
{
    //..editing  board
    array<short, 49> *board = &(status->board);
    swap((*board)[startp], (*board)[endp]);
    if ((*board)[endp]==3){
        status->king_pos=endp;
    }
    // black:
    if ((endp % 7 < 5) && (*board)[endp] == 1 && (*board)[endp + 1] != 1 && (*board)[endp + 2] == 1)
    {
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp + 1] = 0;
    }
    if ((endp % 7 > 1) && (*board)[endp] == 1 && (*board)[endp - 1] != 1 && (*board)[endp - 2] == 1)
    {
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp - 1] = 0;
    }
    if ((endp / 7 < 5) && (*board)[endp] == 1 && (*board)[endp + 7] != 1 && (*board)[endp + 14] == 1)
    {
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp + 7] = 0;
    }
    if ((endp / 7 > 1) && (*board)[endp] == 1 && (*board)[endp - 7] != 1 && (*board)[endp - 14] == 1)
    {
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp - 7] = 0;
    }
    // white:
    if ((*board)[endp]==3 && (endp/7==0 || endp/7==6 || endp%7==0 || endp%7==6 ) )
        status->winner=2;
    if ((endp % 7 < 5) && (*board)[endp] > 1 && (*board)[endp + 1] == 1 && (*board)[endp + 2] > 1)
    {
        status->blackpieces--;
        status->capture = true;
        (*board)[endp + 1] = 0;
    }
    if ((endp % 7 > 1) && (*board)[endp] > 1 && (*board)[endp - 1] == 1 && (*board)[endp - 2] > 1)
    {
        status->blackpieces--;
        status->capture = true;
        (*board)[endp - 1] = 0;
    }
    if ((endp / 7 < 5) && (*board)[endp] > 1 && (*board)[endp + 7] == 1 && (*board)[endp + 14] > 1)
    {
        status->blackpieces--;
        status->capture = true;
        (*board)[endp + 7] = 0;
    }
    if ((endp / 7 > 1) && (*board)[endp] > 1 && (*board)[endp - 7] == 1 && (*board)[endp - 14] > 1)
    {
        status->blackpieces--;
        status->capture = true;
        (*board)[endp - 7] = 0;
    }
    //Calculating interest_value
    if (status->winner){
        status->interest_value=100;
    }
    if (status->capture){
        status->interest_value++;
    }
    if ((*board)[endp]==3){
        status->interest_value++;
    }


    return;
}

void PrintBoard(Status status)
{   
    array<short,49> board=status.board;
    for (int i = 0; i < 49; i++)
    {
        if (i % 7 == 0)
            cout << "\n";
        cout <<board[i] <<" ";
    }
    cout << endl;
}

void CreateMoveStates(Status status, bool player,vector<Status> *possible_moves)
{
    array<short,49> board=status.board;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int stp = i + j * 7; // STart Position
            if ((board[stp] == 1 && player) || (board[stp] > 1 && !player))
            {
                int dif = 0;
                // UP SearcH, DOwn SearcH etc...
                bool upsh = true, dosh = true, lesh = true, rish = true;
                while (upsh || dosh || lesh || rish)
                { // checking possible up moves
                    dif++;

                    int up_place=i - dif + j * 7;
                    if (upsh && board[up_place] == 0 && dif <= i)
                    { // up empty spot
                        Status newstatus = status;
                        newstatus.capture=false;
                        newstatus.interest_value=0;
                        MaterializeMove(&newstatus,stp,up_place);
                        possible_moves->push_back(newstatus);
                    }
                    else
                        upsh = false;
                    
                    int down_place=i + dif + j * 7;
                    if (dosh && board[down_place] == 0 && dif + i <= 6)
                    { // down empty spot
                        Status newstatus = status;
                        newstatus.capture=false;
                        newstatus.interest_value=0;
                        MaterializeMove(&newstatus,stp,down_place);
                        possible_moves->push_back(newstatus);
                    }
                    else
                        dosh = false;

                    int left_place=i + (j - dif) * 7;
                    if (lesh && board[left_place] == 0 && dif <= j)
                    { // left empty spot
                        Status newstatus = status;
                        newstatus.capture=false;
                        newstatus.interest_value=0;
                        MaterializeMove(&newstatus,stp,left_place);
                        possible_moves->push_back(newstatus);
                    }
                    else
                        lesh = false;

                    int right_place=i + (dif + j) * 7;
                    if (rish && board[right_place] == 0 && dif + j <= 6)
                    { // right empty spot
                        Status newstatus = status;
                        newstatus.capture=false;
                        newstatus.interest_value=0;
                        MaterializeMove(&newstatus,stp,right_place);
                        possible_moves->push_back(newstatus);
                    }
                    else
                        rish = false;
                }
            }
        }
    }
    sort(possible_moves->begin(),possible_moves->end(),CompareMoves());
    return ;
}

/*
{0,0,0,1,0,0,0,
0,0,0,1,0,0,0,
0,0,0,2,0,0,0,
1,1,2,3,2,1,1,
0,0,0,2,0,0,0,
0,0,0,1,0,0,0,
0,0,0,1,0,0,0}
*/

array<short, 49> start_ar =
    {0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 2, 0, 0, 0,
     1, 1, 2, 3, 2, 1, 1,
     0, 0, 0, 2, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0};

// returns how good the game is for the black player
int Evaluation(Status status)
{
    int ev = 0;
    ev=status.blackpieces-status.whitepieces;
    return ev;
}


int minmax(int d, int maxd, int current, bool player, stack<Status> *status_stack)
{
    Status status = status_stack->top();

    if (status.winner == 1)
        return 1000 - d;

    if (status.winner == 2)
        return -1000 + d;


    if (d >= maxd)
    {
//        if (!(status.capture))
        return Evaluation(status);
    }
    int evalue;
    if (player == true)
    {
        evalue = -12345;
    }
    else
    {
        evalue = 12345;
    }

    vector<Status> moves;
    CreateMoveStates(status, player, &moves);

    for (Status move : moves)
    {
        status_stack->push(move);
        int move_value = minmax(d + 1, maxd, evalue, !player, status_stack);
        if (player == true)
        {
            evalue = max(evalue, move_value);
            if (evalue > current)
            {
                return evalue;
            }
        }
        if (player == false)
        {
            evalue = min(evalue, move_value);
            if (evalue < current)
            {
                return evalue;
            }
        }
    }
    status_stack->pop();
    return evalue;
}


void CalculateMinMax(Status move, int bestEva, int player,int &result){
    stack<Status> mstack;
    mstack.push(move);
    result = minmax(0, depth, bestEva, !player, &mstack);
}



Status FindBestMove(Status board, bool player)
{
    //    cout<<"Starting Search \n";
    Status bestMove=board;
    int bestEva;
    if (player == true)
    {
        bestEva = -10000;
    }
    else
    {
        bestEva = 10000;
    }

    vector<Status> moves;
    CreateMoveStates(board, player, &moves);
// Multithreading!!!

    int moves_size=moves.size();
    vector<thread> threads;

    int move_results[moves_size];

    for( int i=0; i<moves_size; i++){
        threads.push_back( thread(CalculateMinMax,moves[i],bestEva,player,ref(move_results[i])));
    }

    for(thread &t : threads){
        if (t.joinable())
            t.join();
    }

    for (int i=0; i<moves_size; i++)
    {
        if ((move_results[i] > bestEva && player == true) || (move_results[i] < bestEva && player == false))
        {
            bestEva = move_results[i];
            bestMove = moves[i];
        }
    }


// DANGER
/*    
for (Status move : moves)
    {
        stack<Status> mstack;
        mstack.push(move);
        int move_value = minmax(0, depth, bestEva, !player, &mstack);
        //        cout<<"move value turned to be "<<move_value<<endl;
        if ((move_value > bestEva && player == true) || (move_value < bestEva && player == false))
        {
            bestEva = move_value;
            bestMove = move;
        }
    }
*/
    //    cout<<endl<<"BEST EVA: " << bestEva<<endl<<endl;
    return bestMove;
}

int main()
{
    cout << "S/\n";
    bool player = true;
    Status starting(start_ar);
    cout<<"Enter desired depth (depth 6 will take approximately a minute for each move):\n";
    cin>>depth;
    PrintBoard(starting);

    while (starting.winner==0)
    {
        starting = FindBestMove(starting, player);
        PrintBoard(starting);
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        cin >> x1 >> y1 >> x2 >> y2;
        MaterializeMove(&starting, x1 - 1 + (7 - y1) * 7, x2 - 1 + (7 - y2) * 7);
        PrintBoard(starting);
    }
    return 0;
}
