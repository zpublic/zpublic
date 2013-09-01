#pragma once
#include <time.h>

void AnimateDraw_StaggeredHorizontal(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 10)
{
    int i, j;
    for (i = 0;i <= nHeight; i += 2)
    {
        for (j = i; j > 0; j -= 2)
        {
            BitBlt(hdc, 0, j - 1, nWidth, 1, hdcMem, 0, nHeight - (i - j - 1), SRCCOPY);
            BitBlt(hdc, 0, nHeight - j, nWidth, 1, hdcMem,	0, i - j, SRCCOPY); 
        }
        Sleep(nIntervalTime);
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_StaggeredVertical(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 10)
{
    int i, j;
    for (i = 0; i <= nWidth; i += 2)
    {
        for (j = i; j > 0; j -= 2)
        {
            BitBlt(hdc, j - 1, 0, 1, nHeight, hdcMem, nWidth - (i - j - 1), 0, SRCCOPY);
            BitBlt(hdc, nWidth - j, 0, 1, nHeight, hdcMem, i - j, 0, SRCCOPY); 
        }
        Sleep(nIntervalTime);
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_JalousieHorizontal(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 15,
    UINT nJalousieNum = 10)
{
    int nJalousieHeight = nHeight / nJalousieNum;
    for (int i = 0; i <= nJalousieHeight; i++)
    {
        for(UINT j = 0; j < nJalousieNum; j++)
        {
            BitBlt(hdc, 0, j * nJalousieHeight, nWidth, i, hdcMem, 0, j * nJalousieHeight, SRCCOPY);
        }
        Sleep(nIntervalTime);
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_JalousieVertical(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 15,
    UINT nJalousieNum = 10)
{
    int nStep = nWidth / nJalousieNum;
    for (int i = 0; i <= nStep; i++)
    {
        for(UINT j = 0;j < nJalousieNum; j++)
        {
            BitBlt(hdc, j * nStep, 0, i, nHeight,hdcMem, j * nStep, 0, SRCCOPY);
        }
        Sleep(nIntervalTime);
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_RandomBlocks(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 15,
    int nRowBlocks = 10,
    int nColBlocks = 10)
{
    int nStepRow, nStepCol, nDisplayCount;
    int nSumBlocks = nRowBlocks * nColBlocks;
    bool *pFlagArray = new bool[nSumBlocks];
    memset(pFlagArray, 0, sizeof(bool) * nSumBlocks);

    nStepRow = nWidth / nRowBlocks;
    nStepCol = nHeight / nColBlocks;
    srand((unsigned)time(NULL));
    nDisplayCount = 0;
    while (true) 
    {
        int row = rand() % nRowBlocks;
        int col = rand() % nColBlocks;
        if (pFlagArray[row * nColBlocks + col])
            continue;
        pFlagArray[row * nColBlocks + col] = true;

        BitBlt(hdc, row * nStepRow, col * nStepCol, nStepRow, nStepCol,
            hdcMem, row * nStepRow, col * nStepCol, SRCCOPY);
        nDisplayCount++;
        if (nDisplayCount >= nSumBlocks)
            break;
        Sleep(nIntervalTime); 
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_FlyingTopToBottom(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 2,
    BOOL bFade = TRUE)
{
    int j;
    if (bFade)
    {
        for (j = 0; j <= nHeight; j++)
        {
            BitBlt(hdc, 0, 0, nWidth, j, hdcMem, 0, 0, SRCCOPY);
            Sleep(nIntervalTime);
        }
    }
    else
    {
        for (j = 0; j <= nHeight; j++)
        {
            BitBlt(hdc, 0, 0, nWidth, j, hdcMem, 0, nHeight - j, SRCCOPY);
            Sleep(nIntervalTime);
        }
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_FlyingBottomToTop(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 2,
    BOOL bFade = TRUE)
{
    int j;
    if (bFade)
    {
        for (j = nHeight; j >= 0; j--)
        {
            BitBlt(hdc, 0, j, nWidth, nHeight - j, hdcMem, 0, j, SRCCOPY);
            Sleep(nIntervalTime);
        }
    }
    else
    {
        for (j = nHeight; j >= 0; j--)
        {
            BitBlt(hdc, 0, j, nWidth, nHeight - j, hdcMem, 0, 0, SRCCOPY);
            Sleep(nIntervalTime);
        }
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_FlyingLeftToRight(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 2,
    BOOL bFade = TRUE)
{
    int i;
    if (bFade)
    {
        for (i = 0; i <= nWidth; i++)
        {
            BitBlt(hdc, 0, 0, i, nHeight, hdcMem, 0, 0, SRCCOPY); 
            Sleep(nIntervalTime);
        }
    }
    else
    {
        for (i = 0; i <= nWidth; i++)
        {
            BitBlt(hdc, 0, 0, i, nHeight, hdcMem, nWidth - i, 0, SRCCOPY); 
            Sleep(nIntervalTime);
        }
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}

void AnimateDraw_FlyingRightToLeft(
    HDC hdc,
    HDC hdcMem,
    int nWidth,
    int nHeight,
    UINT nIntervalTime = 2,
    BOOL bFade = TRUE)
{
    int i;
    if (bFade)
    {
        for (i = nWidth; i >= 0; i--)
        {
            BitBlt(hdc, i, 0, nWidth - i, nHeight, hdcMem, i, 0, SRCCOPY);
            Sleep(nIntervalTime);
        }
    }
    else
    {
        for (i = nWidth; i >= 0; i--)
        {
            BitBlt(hdc, i, 0, nWidth - i, nHeight, hdcMem, 0, 0, SRCCOPY);
            Sleep(nIntervalTime);
        }
    }
    BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);
}
