#include <bits/stdc++.h>
using namespace std;

int depth ;

class Status
{
public:
    array<short, 49> board; // it is the basic board[49] with 0 at empty spaces 1->black 2->white pieces
    int king_pos; // it contains the position of the king in the board (0->top left space, 48 bottom right)
// king pos will just be now the first element of an array<int,13>
    int winner = 0; // if the game has concluded with a winner 1->black 2->white
    int whitepieces = 4; //the number of white pieces
    int blackpieces = 8; //the number of black pieces
    int interest_value=0; // how interesting is the move (λογικά θα το αλλάξουμε αυτό, γιατί θα κοιτάμε τις κινήσεις μία-μία
//δηλαδή στο minmax θα έχουμε μία for loop for pieces -> for 1->24 και θα ζητάμε μία μία τις κινήσεις 1,2,3,4,5
//οι κινήσεις από 0->5 θα είναι προς τα πάνω από 6->11 προς τα δεξιά 12->17 αριστερά, 18->23 κάτω. Οπότε δε θα υπάρχει χρόνος
//να shortάρουμε τις κινήσεις σχετικά με το interest_value τους.
    bool capture = false; // was there a capture in this turn? The minmax should never stop evaluating when there was a capture made.
    Status(array<short, 49> boarda){
        board=boarda;//deep copy (std:array)
        whitepieces=0;//counting whites
        blackpieces=0;//counting black
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

//αυτή η συνάρτηση υπήρχε μόνο και μόνο για να σορτάρω τις κινήσεις και να τις βλέπω από την πιο ενδιαφέρουσα μέχρι την πιο βαρετή.
//Ωστόοσ αυτό προΰποθέτει να έχουμε ένα vector με όλες τις κινήσεις. Το οποίο προΰποθέτει σε κάθε κλήση του minmax να δημιουργούμε
//ένα vector. Υποψιάζομαι πως αυτό καθυστερεί, για αυτό απλώς θα τις ζητάμε από το minmax με έναν κωδικό αριθμό 0-24.
struct CompareMoves {
    bool operator()(const Status& a, const Status& b) const {
        return a.interest_value > b.interest_value; // Change to < for min-heap behavior
    }
};

// EDITS the Status instance sent according to the move inputed.
//Returns nothing, as the changes are done to the pointed class
void MaterializeMove(Status *status, int startp, int endp)
{
    //..editing  board
    array<short, 49> *board = &(status->board);
    swap((*board)[startp], (*board)[endp]);
    if ((*board)[endp]==3){
        status->king_pos=endp;
    }
    // black:
    //the if statements are correct, it is inside the conditions we need to change things.
    if ((endp % 7 < 5) && (*board)[endp] == 1 && (*board)[endp + 1] > 1 && (*board)[endp + 2] == 1)
    {
        //change the status, the white count, check for winners and edit the board 
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp + 1] = 0;
    }
    if ((endp % 7 > 1) && (*board)[endp] == 1 && (*board)[endp - 1] > 1 && (*board)[endp - 2] == 1)
    {
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp - 1] = 0;
    }
    if ((endp / 7 < 5) && (*board)[endp] == 1 && (*board)[endp + 7] > 1 && (*board)[endp + 14] == 1)
    {
        status->capture = true;
        status->whitepieces--;
        if ((*board)[endp + 1] == 3)
            status->winner = 1;
        (*board)[endp + 7] = 0;
    }
    if ((endp / 7 > 1) && (*board)[endp] == 1 && (*board)[endp - 7] > 1 && (*board)[endp - 14] == 1)
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

//This function just works, don't change it
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


// this functions DOESN't CREATE A NEW VECTOR! instead it writes in the inputed pointed vector (possible moves)
void CreateMoveStates(Status status, bool player,vector<Status> *possible_moves)
{
    //we will need to drastically change this function
    //instead of outputing all possible moves, this function needs to take a code number of a move, the position of a piece,
    //and then edit the board accordingly.
    //It will also return true if the move has been completed and false if there was something blocking it
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

//starting position
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

//Changes we need to make:
//There will be NO STACK just the same Status instance that will be edited and undoed.
//Instead of creating all moves in a vector, we will have a for loop that will ask for every move 0-24
//If CreateMoveStates() returns false in a move (3), instead of trying the next one(4) we will have to change direction.
//the next move we should try is in another direction. (0-5)(6-11)(12-17)(18-13) are all differrent directions, so we just have to do
// (current move/6)+6 so if tried move 3 and failed, the next one we try is 6. If we tried 12 and failed, the next one is 18.
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

//It is a very basic function, it is necessary for the multithreading process.
void CalculateMinMax(Status move, int bestEva, int player,int &result){
    stack<Status> mstack;//will be removed, minmax will only take a Status class as input, not a whole stack
    mstack.push(move);//instead of pushing it to the stack, we just need to pass the "move"
    result = minmax(0, depth, bestEva, !player, &mstack);
}


// no major changes are needed in this function.
// it creates all possible moves and finds the best one using minmax
// We will need to change the way it finds all those moves, using the code numbers instead of vectors
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
