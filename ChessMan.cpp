#include "ChessMan.h"
#include <QPixmap>
ChessMan::ChessMan()
{

}
void ChessMan::init(int id)
{
    _id=id;
    _dead=false;
    _side=id<16;
    typedef struct{
        int row,col;
        TYPE type;
        QPixmap img,imgSl;
    } initPos;
    initPos pos[32]={
        {0,0,ChessMan::CHE,QPixmap(":/chess/CHE_B.GIF"),QPixmap(":/chess/CHE_BS.GIF")},
        {0,1,ChessMan::MA,QPixmap(":/chess/MA_B.GIF"),QPixmap(":/chess/MA_BS.GIF")},
        {0,2,ChessMan::XIANG,QPixmap(":/chess/XIANG_B.GIF"),QPixmap(":/chess/XIANG_BS.GIF")},
        {0,3,ChessMan::SHI,QPixmap(":/chess/SHI_B.GIF"),QPixmap(":/chess/SHI_BS.GIF")},
        {0,4,ChessMan::JIANG,QPixmap(":/chess/JIANG_B.GIF"),QPixmap(":/chess/JIANG_BS.GIF")},
        {0,5,ChessMan::SHI,QPixmap(":/chess/SHI_B.GIF"),QPixmap(":/chess/SHI_BS.GIF")},
        {0,6,ChessMan::XIANG,QPixmap(":/chess/XIANG_B.GIF"),QPixmap(":/chess/XIANG_BS.GIF")},
        {0,7,ChessMan::MA,QPixmap(":/chess/MA_B.GIF"),QPixmap(":/chess/MA_BS.GIF")},
        {0,8,ChessMan::CHE,QPixmap(":/chess/CHE_B.GIF"),QPixmap(":/chess/CHE_BS.GIF")},
        {2,1,ChessMan::PAO,QPixmap(":/chess/PAO_B.GIF"),QPixmap(":/chess/PAO_BS.GIF")},
        {2,7,ChessMan::PAO,QPixmap(":/chess/PAO_B.GIF"),QPixmap(":/chess/PAO_BS.GIF")},
        {3,0,ChessMan::BING,QPixmap(":/chess/BING_B.GIF"),QPixmap(":/chess/BING_BS.GIF")},
        {3,2,ChessMan::BING,QPixmap(":/chess/BING_B.GIF"),QPixmap(":/chess/BING_BS.GIF")},
        {3,4,ChessMan::BING,QPixmap(":/chess/BING_B.GIF"),QPixmap(":/chess/BING_BS.GIF")},
        {3,6,ChessMan::BING,QPixmap(":/chess/BING_B.GIF"),QPixmap(":/chess/BING_BS.GIF")},
        {3,8,ChessMan::BING,QPixmap(":/chess/BING_B.GIF"),QPixmap(":/chess/BING_BS.GIF")},

        {9,8,ChessMan::CHE,QPixmap(":/chess/CHE_R.GIF"),QPixmap(":/chess/CHE_RS.GIF")},
        {9,7,ChessMan::MA,QPixmap(":/chess/MA_R.GIF"),QPixmap(":/chess/MA_RS.GIF")},
        {9,6,ChessMan::XIANG,QPixmap(":/chess/XIANG_R.GIF"),QPixmap(":/chess/XIANG_RS.GIF")},
        {9,5,ChessMan::SHI,QPixmap(":/chess/SHI_R.GIF"),QPixmap(":/chess/SHI_RS.GIF")},
        {9,4,ChessMan::JIANG,QPixmap(":/chess/JIANG_R.GIF"),QPixmap(":/chess/JIANG_RS.GIF")},
        {9,3,ChessMan::SHI,QPixmap(":/chess/SHI_R.GIF"),QPixmap(":/chess/SHI_RS.GIF")},
        {9,2,ChessMan::XIANG,QPixmap(":/chess/XIANG_R.GIF"),QPixmap(":/chess/XIANG_RS.GIF")},
        {9,1,ChessMan::MA,QPixmap(":/chess/MA_R.GIF"),QPixmap(":/chess/MA_RS.GIF")},
        {9,0,ChessMan::CHE,QPixmap(":/chess/CHE_R.GIF"),QPixmap(":/chess/CHE_RS.GIF")},
        {7,7,ChessMan::PAO,QPixmap(":/chess/PAO_R.GIF"),QPixmap(":/chess/PAO_RS.GIF")},
        {7,1,ChessMan::PAO,QPixmap(":/chess/PAO_R.GIF"),QPixmap(":/chess/PAO_RS.GIF")},
        {6,8,ChessMan::BING,QPixmap(":/chess/BING_R.GIF"),QPixmap(":/chess/BING_RS.GIF")},
        {6,6,ChessMan::BING,QPixmap(":/chess/BING_R.GIF"),QPixmap(":/chess/BING_RS.GIF")},
        {6,4,ChessMan::BING,QPixmap(":/chess/BING_R.GIF"),QPixmap(":/chess/BING_RS.GIF")},
        {6,2,ChessMan::BING,QPixmap(":/chess/BING_R.GIF"),QPixmap(":/chess/BING_RS.GIF")},
        {6,0,ChessMan::BING,QPixmap(":/chess/BING_R.GIF"),QPixmap(":/chess/BING_RS.GIF")}
    };
    _curRow=pos[id].row;
    _curCol=pos[id].col;
    _type=pos[id].type;
    _img=pos[id].img;
    _imgSl=pos[id].imgSl;
}
