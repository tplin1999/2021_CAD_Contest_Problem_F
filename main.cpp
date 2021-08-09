# include <iostream>
# include <fstream>
# include <vector>

using namespace std ;

// 儲存輸入檔案的基本資料
struct V_INFORMATION
{
    vector < vector < string > > dut_assign ;
    vector < string > dut_io_and_tb ;
} ;// end V_INFORMATION

void erase_tab ( string & str ) ;
void erase_space ( string & str ) ;
V_INFORMATION read_file ( ifstream & fin ) ;

void delete_and_redundant ( vector < string > & vec ) ;
void delete_or_redundant ( vector < string > & vec ) ;
void delete_xor_redundant ( vector < string > & vec ) ;
void delete_not_redundant ( vector < string > & vec ) ;
void delete_operator_redundant ( vector < vector < string > > & vec ) ;

bool delete_0_and_1_redundant ( vector < vector < string > > & vec ) ;

void optimized ( vector < vector < string > > & vec ) ;

vector < string > find_tmp ( vector < vector < string > > & vec ) ;
void output_file ( ofstream & fout , V_INFORMATION v_information ) ;

void print_vec ( vector < string > vec ) ;
void print_vec ( vector < vector < string > > vec ) ;

int main ( int argc , char * argv [ ] )
{
    ifstream fin ( argv [ 1 ] , ios :: in ) ;
    ofstream fout ( argv [ 2 ] , ios :: out ) ;

    V_INFORMATION v_information = read_file ( fin ) ;
    
    optimized ( v_information . dut_assign ) ;

    output_file ( fout , v_information ) ;

    fin . close ( ) ;

    return 0 ;
}

// 去除字串中的所有tab鍵
void erase_tab ( string & str )
{
    int pos = 0 ;

    while ( ( pos = str . find_first_of ( "\t" , pos ) ) >= 0 )
    {
        str = str . erase ( pos , 1 ) ;
    }
}// end erase_tab

// 去除字串中的所有space鍵
void erase_space ( string & str )
{
    int pos = 0 ;

    while ( ( pos = str . find_first_of ( " " , pos ) ) >= 0 )
    {
        str = str . erase ( pos , 1 ) ;
    }
}// end erase_space

// 切割dut的assign部分
vector < string > cut_assign ( string str )
{
    vector < string > cutted_assign ;
    str = str . substr ( 6 ) ;

    int pos_equal = str . find_first_of ( "=" ) ;
    int pos_operator = str . find_first_of ( "&|^" ) ;
    int pos_semicolon = str . find_first_of ( ";" ) ;

    if ( pos_operator >= 0 )
    {
        cutted_assign . push_back ( str . substr ( 0 , pos_equal ) ) ;
        cutted_assign . push_back ( str . substr ( pos_equal + 1 , pos_operator - pos_equal - 1 ) ) ;
        cutted_assign . push_back ( str . substr ( pos_operator , 1 ) ) ;
        cutted_assign . push_back ( str . substr ( pos_operator + 1 , pos_semicolon - pos_operator - 1 ) ) ;
    }
    else
    {
        cutted_assign . push_back ( str . substr ( 0 , pos_equal ) ) ;
        cutted_assign . push_back ( str . substr ( pos_equal + 1 , pos_semicolon - pos_equal - 1 ) ) ;
    }
    
    return cutted_assign ;
}// end cut_assign

// 讀取檔案的內容 
V_INFORMATION read_file ( ifstream & fin )
{
    vector < vector < string > > dut_assign ;
    vector < string > dut_io_and_tb ; 

    string input_file_line_by_line ;

    while ( getline ( fin , input_file_line_by_line ) )
    {// 讀檔迴圈開始
        if ( input_file_line_by_line == "" )
        {// 遇到空白行時直接儲存到dut_io_and_tb，和else做的事情一模一樣，另外寫一個判斷是因為，避免下面兩個else if判斷時，出現bug
            dut_io_and_tb . push_back ( input_file_line_by_line ) ;
        }
        else if ( input_file_line_by_line . substr ( 1 , 4 ) == "wire" )
        {// wire跳過不處理
            continue ;
        }
        else if ( input_file_line_by_line . substr ( 1 , 6 ) == "assign" )
        {// assign儲存到dut_assign裡
            erase_tab ( input_file_line_by_line ) ;
            erase_space ( input_file_line_by_line ) ;
            dut_assign . push_back ( cut_assign ( input_file_line_by_line ) ) ;
        }
        else
        {// 除了wire和assign以外的東西儲存到dut_io_and_tb裡(空白行在if判斷式中，已經儲存到dut_io_and_tb裡了)
            dut_io_and_tb . push_back ( input_file_line_by_line ) ;
        }
    }// 讀檔迴圈結束

    V_INFORMATION v_information ;
    v_information . dut_assign = dut_assign ;
    v_information . dut_io_and_tb = dut_io_and_tb ;
    return v_information ;
}// end read_file

// 處理and裡多餘的東西
void delete_and_redundant ( vector < string > & vec )
{
    if ( vec [ 1 ] == "1'b0" || vec [ 3 ] == "1'b1" )
    {
        vec . erase ( vec . begin ( ) + 2 , vec . begin ( ) + 4 ) ;
    }
    else if ( vec [ 1 ] == "1'b1" || vec [ 3 ] == "1'b0" )
    {
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 3 ) ;
    }
    else
    {
        ;
    }
}// end delete_and_redundant

// 處理or裡多餘的東西
void delete_or_redundant ( vector < string > & vec )
{
    if ( vec [ 1 ] == "1'b0" || vec [ 3 ] == "1'b1" )
    {
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 3 ) ;
    }
    else if ( vec [ 1 ] == "1'b1" || vec [ 3 ] == "1'b0" )
    {
        vec . erase ( vec . begin ( ) + 2 , vec . begin ( ) + 4 ) ;
    }
    else
    {
        ;
    }
}// end delete_or_redundant

// 處理xor裡多餘的東西
void delete_xor_redundant ( vector < string > & vec )
{
    if ( vec [ 1 ] == "1'b0" && vec [ 3 ] == "1'b0" )
    {
        vec . erase ( vec . begin ( ) + 2 , vec . begin ( ) + 4 ) ;
    }
    else if ( vec [ 1 ] == "1'b0" && vec [ 3 ] == "1'b1" )
    {
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 3 ) ;
    }
    else if ( vec [ 1 ] == "1'b1" && vec [ 3 ] == "1'b0" )
    {
        vec . erase ( vec . begin ( ) + 2 , vec . begin ( ) + 4 ) ;
    }
    else if ( vec [ 1 ] == "1'b1" && vec [ 3 ] == "1'b1" )
    {
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 4 ) ;
        vec . insert ( vec . begin ( ) + 1 , "1'b0" ) ;
    }
    else if ( vec [ 1 ] == "1'b0" )
    {
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 3 ) ;
    }
    else if ( vec [ 1 ] == "1'b1" )
    {
        string temp = vec [ 3 ] ;
        temp . insert ( 0 , "~" ) ;
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 4 ) ;
        vec . insert ( vec . begin ( ) + 1 , temp ) ;
    }
    else if ( vec [ 3 ] == "1'b0" )
    {
        vec . erase ( vec . begin ( ) + 2 , vec . begin ( ) + 4 ) ;
    }
    else if ( vec [ 3 ] == "1'b1" )
    {
        string temp = vec [ 1 ] ;
        temp . insert ( 0 , "~" ) ;
        vec . erase ( vec . begin ( ) + 1 , vec . begin ( ) + 4 ) ;
        vec . insert ( vec . begin ( ) + 1 , temp ) ;
    }
    else
    {
        ;
    }
}// end delete_xor_redundant

// 處理not裡多餘的東西
void delete_not_redundant ( vector < string > & vec )
{
    for ( int i = 0 ; i < vec.size ( ) ; i ++ )
    {
        if ( vec [ i ] == "~1'b0" )
        {
            vec . erase ( vec . begin ( ) + i ) ;
            vec . insert ( vec . begin ( ) + i , "1'b1" ) ;
        }
        if ( vec [ i ] == "~1'b1" )
        {
            vec . erase ( vec . begin ( ) + i ) ;
            vec . insert ( vec . begin ( ) + i , "1'b0" ) ;
        }
    }
}// end delete_not_redundant

// 刪除多餘的operator，例：a&1 = a , a|1 = 1。
void delete_operator_redundant ( vector < vector < string > > & vec )
{
    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        delete_not_redundant ( vec [ i ] ) ;

        if ( vec [ i ] . size ( ) == 4 )
        {// 確認這個vector裡有operator
            if ( vec [ i ] [ 2 ] == "&" )
            {
                delete_and_redundant ( vec [ i ] ) ;
            }
            else if ( vec [ i ] [ 2 ] == "|" )
            {
                delete_or_redundant ( vec [ i ] ) ;
            }
            else if ( vec [ i ] [ 2 ] == "^" )
            {
                delete_xor_redundant ( vec [ i ] ) ;
            }
            else
            {
                ;
            }
        }
    }
}// end delete_operator_redundant

// 刪除1'b0和1'b1
bool delete_0_and_1_redundant ( vector < vector < string > > & vec )
{
    bool big_change = false ;
    bool change = true ;
    
    while ( change == true )
    {
        change = false ;

        for ( int i = 0 ; i < vec . size ( ) ; i ++ )
        {
            if ( vec [ i ] [ 1 ] == "1'b0" || vec [ i ] [ 1 ] == "1'b1" )
            {
                for ( int j = 0 ; j < vec . size ( ) ; j ++ )
                {
                    if ( j == i )
                    {
                        continue ;
                    }
                    else
                    {
                        for ( int k = 0 ; k < vec [ j ] . size ( ) ; k ++ )
                        {
                            if ( vec [ j ] [ k ] == vec [ i ] [ 0 ] )
                            {
                                vec [ j ] . erase ( vec [ j ] . begin ( ) + k ) ;
                                vec [ j ] . insert ( vec [ j ] . begin ( ) + k , vec [ i ] [ 1 ] ) ;
                                change = true ;
                                big_change = true ;
                            }
                        }
                    }
                }
            }     
        }
    }

    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        if ( vec [ i ] . size ( ) == 2 && ( vec [ i ] [ 1 ] == "1'b0" || vec [ i ] [ 1 ] == "1'b1" ) )
        {
            if ( vec [ i ] [ 0 ] . substr ( 0 , 7 ) == "origtmp" )
            {
                vec . erase ( vec. begin ( ) + i ) ;
            }
        }
    }

    return big_change ;
}// end delete_0_and_1_redundant

// 最大的optimized function
void optimized ( vector < vector < string > > & vec )
{
    bool big_change = true ;

    while ( big_change == true )
    {
        delete_operator_redundant ( vec ) ;
        big_change = delete_0_and_1_redundant ( vec ) ;
    }   
}// end optimized

// 找到tmp
vector < string > find_tmp ( vector < vector < string > > & vec )
{
    vector < string > tmp ;

    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        if ( vec [ i ] [ 0 ] . substr ( 0 , 7 ) == "origtmp" )
        {
            tmp .push_back ( vec [ i ] [ 0 ] ) ;
        }
    }

    return tmp ;
}// end find_tmp

// 輸出檔案
void output_file ( ofstream & fout , V_INFORMATION v_information )
{
    vector < string > tmp = find_tmp ( v_information . dut_assign ) ;

    fout << v_information . dut_io_and_tb [ 0 ] << endl ;
    fout << v_information . dut_io_and_tb [ 1 ] << endl ;
    fout << v_information . dut_io_and_tb [ 2 ] << endl ;

    for ( int i = 0 ; i < tmp . size ( ) ; i ++ )
    {
        fout << "\twire " << tmp [ i ] << endl ;
    }

    for ( int i = 0 ; i < v_information . dut_assign . size ( ) ; i ++ )
    {
        switch ( v_information . dut_assign [ i ] . size ( ) )
        {
        case 2 :
            fout << "\tassign " << v_information . dut_assign [ i ] [ 0 ] << \
            " = " << v_information . dut_assign [ i ] [ 1 ] << ";" << endl ;
            break ;
        
        case 4 :
            fout << "\tassign " << v_information . dut_assign [ i ] [ 0 ] << \
            " = " << v_information . dut_assign [ i ] [ 1 ] << \
            " " << v_information . dut_assign [ i ] [ 2 ] << \
            " " << v_information . dut_assign [ i ] [ 3 ] << ";" << endl ;
            break ;
        
        default :
            break ;
        }
    }

    for ( int i = 3 ; i < v_information . dut_io_and_tb . size ( ) ; i ++ )
    {
        fout << v_information . dut_io_and_tb [ i ] << endl ;
    }
}// end output_file

// 開發者測試用function
void print_vec ( vector < string > vec )
{
    cout << "vector < string > : " << endl ;

    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        cout << vec [ i ] << endl ;
    }
}// end print_vec

// 開發者測試用function
void print_vec ( vector < vector < string > > vec )
{
    cout << "vector < vector < string > > : " << endl ;

    for ( int i = 0 ; i < vec . size ( ) ; i ++ )
    {
        cout << i << " : " ;

        for ( int j = 0 ; j < vec [ i ] . size ( ) ; j ++ )
        {
            cout << vec [ i ] [ j ] << " " ;
        }

        cout << endl ;
    }
}// end print_vec