//
//  rs.cpp
//  QPSKLib
//
//  Created by konstantin gedalin on 8/14/12.
//  Copyright (c) 2012 Frodo. All rights reserved.
//

#include "sci_rs.h"
#include <iostream>
#include <vector>


namespace sci_modem
{
    
namespace sci_common{
rs::rs()
{
generate_gf();
generator_polynomial();
};



/******************************************************************
 generate Galois field 
 function : void generate GF() 
 from the irreducible polynomial 
 p(X) in polynomialCoeffs[0]..polynomialCoeffs[mm]
 lookup tables:  index->polynomial form   alpha_to[] contains j=alpha^i;
 polynomial form -> index form  index_of[j=alpha**i] = i
 alpha=2 is the primitive element of GF(2**mm)
 *************************************************************************/

void rs::generate_gf()
{
    int mask = 1 ;
    alpha_to[kGFAlpha] = 0 ;
    for (int i=0; i<kGFAlpha; i++)
    { 
        alpha_to[i] = mask ;
        index_of[alpha_to[i]] = i ;
        if (polynomialCoeffs[i])
            alpha_to[kGFAlpha] ^= mask ;
        mask <<= 1 ;
    }
    index_of[alpha_to[kGFAlpha]] = kGFAlpha ;
    mask >>= 1 ;
    for (int i=kGFAlpha+1; i<kCodeWordLength; i++)
    { if (alpha_to[i-1] >= mask)
        alpha_to[i] = alpha_to[kGFAlpha] ^ ((alpha_to[i-1]^mask)<<1) ;
    else alpha_to[i] = alpha_to[i-1]<<1 ;
        index_of[alpha_to[i]] = i ;
    }
    index_of[0] = -1 ;
}

/* 
 Obtain the generator polynomial of the kNumberOfErrorsToCorrect-error correcting, length
 kCodeWordLength=(2**mm -1) Reed Solomon code  from the product of (X+alpha**i), i=1..2*kNumberOfErrorsToCorrect
 */
void rs:: generator_polynomial()
{
    primitive[0] = 2 ;    /* primitive element alpha = 2 */
    primitive[1] = 1 ;    /* g(x) = (X+alpha) initially */
    for (int i=2; i<=kCodeWordLength-kNumberOfDataBits; i++)
    { 
        primitive[i] = 1 ;
        for (int j=i-1; j>0; --j)
            if (primitive[j] != 0)  
                primitive[j] = primitive[j-1]^ alpha_to[(index_of[primitive[j]]+i)%kCodeWordLength] ;
            else primitive[j] = primitive[j-1] ;
        primitive[0] = alpha_to[(index_of[primitive[0]]+i)%kCodeWordLength] ;
    }
    
    for (int i=0; i<=kCodeWordLength-kNumberOfDataBits; i++)  primitive[i] = index_of[primitive[i]] ;
}
void rs::encode_rs(std::vector<int> &data,std::vector<int> &bb)
{
    int i,j ;
   
    
    for (i=0; i<kCodeWordLength-kNumberOfDataBits; i++)   bb[i] = 0 ;
    for (i=kNumberOfDataBits-1; i>=0; i--)
    {
        int feedback = index_of[data[i]^bb[kCodeWordLength-kNumberOfDataBits-1]] ;
        if (feedback != -1)
        {
            for (j=kCodeWordLength-kNumberOfDataBits-1; j>0; j--)
            {
            if (primitive[j] != -1)
                bb[j] = bb[j-1]^alpha_to[(primitive[j]+feedback)%kCodeWordLength] ;
            else
                bb[j] = bb[j-1] ;
                
            }
            bb[0] = alpha_to[(primitive[0]+feedback)%kCodeWordLength] ;
        }
        else
        {
            for (j=kCodeWordLength-kNumberOfDataBits-1; j>0; j--)
            bb[j] = bb[j-1] ;
            bb[0] = 0 ;
        }
    }
} 

int  rs::decode_rs(std::vector<int> &recd)
{

    iLasterrorNum=0;
    if(FormSyndromes(recd)==NO_ERRORS){
        UpdateRecordedData(recd);
        return NO_ERRORS;
    }
    int u=computeerors();
    iLasterrorNum=l[u];
    if (iLasterrorNum>kNumberOfErrorsToCorrect)  //too much errors
    {    /* elp has degree has degree >kNumberOfErrorsToCorrect hence cannot solve */
     
       UpdateRecordedData(recd);
       return TOO_MUCH_ERRORS; 
    }
    
   if( calculateroots(u)!=iLasterrorNum)
   {    /* elp has degree has degree >kNumberOfErrorsToCorrect hence cannot solve */
       l[u]=10;
       UpdateRecordedData(recd); 
       return TOO_MUCH_ERRORS; 
   }
    /* form polynomial z(x) */
    for (int i=1; i<=iLasterrorNum; i++)        /* Z[0] = 1 always - do not need */
    { 
        if ((s[i]!=-1) && (elp[u][i]!=-1))
        z[i] = alpha_to[s[i]] ^ alpha_to[elp[u][i]] ;
        else if((s[i]!=-1) && (elp[u][i]==-1))
        z[i] = alpha_to[s[i]] ;
        else if ((s[i]==-1) && (elp[u][i]!=-1))
        z[i] = alpha_to[elp[u][i]] ;
        else
        z[i] = 0 ;
        
        for (int j=1; j<i; j++)
        if ((s[j]!=-1) && (elp[u][i-j]!=-1))
         z[i] ^= alpha_to[(elp[u][i-j] + s[j])%kCodeWordLength] ;
         z[i] = index_of[z[i]] ;         /* put into index form */
        }
                
        /* evaluate errors at locations given by error location numbers loc[i] */
        for (int i=0; i<kCodeWordLength; i++)
        {
          err[i] = 0 ;
          if (recd[i]!=-1)        /* convert recd[] to polynomial form */
             recd[i] = alpha_to[recd[i]] ;
             else  recd[i] = 0 ;
        }
        for (int i=0; i<l[u]; i++)    /* compute numerator of error term first */
        { 
            int error=1;
            for (int j=1; j<=l[u]; j++)
            if (z[j]!=-1)
                error ^= alpha_to[(z[j]+j*root[i])%kCodeWordLength] ;
            
           
            if(error!=0) 
            { 
                error=index_of[error];
                int q = 0 ;     /* form denominator of error term */
                for (int j=0; j<l[u]; j++)
                {
                    if (j!=i)
                     q += index_of[1^alpha_to[(loc[j]+root[i])%kCodeWordLength]] ;
                }
                q = q % kCodeWordLength ;
                error = alpha_to[(error-q+kCodeWordLength)%kCodeWordLength] ;
               recd[loc[i]] ^= error ;  /*recd[i] must be in polynomial form */
                
            
            }
        }
       
    
    return ERRORS_CORRECTED;
}


void rs::UpdateRecordedData(std::vector<int> &recd)
{
    for (int i=0; i<kCodeWordLength; ++i)
    {
        if (recd[i]!=-1)        /* convert recd[] to polynomial form */
            recd[i] = alpha_to[recd[i]] ;
        else  recd[i] = 0 ;
    }
}

int  rs::FormSyndromes(std::vector<int> &recd)
{
    int syn_error=0;
    for (int i=1; i<=kCodeWordLength-kNumberOfDataBits; i++){ 
        s[i] = 0 ;
        for (int j=0; j<kCodeWordLength; j++)
            if (recd[j]!=-1)
                s[i] ^= alpha_to[(recd[j]+i*j)%kCodeWordLength] ;      /* recd[j] in index form */
        /* convert syndrome from polynomial form to index form  */
        if (s[i]!=0)  syn_error=1 ;        /* set flag if non-zero syndrome => error */
        s[i] = index_of[s[i]] ;
    } 
    return syn_error;   
}
/* compute the error location polynomial via the Berlekamp iterative algorithm,
 following the terminology of Lin and Costello :   d[u] is the 'mu'th
 discrepancy, where u='mu'+1 and 'mu' (the Greek letter!) is the step number
 ranging from -1 to 2*kNumberOfErrorsToCorrect (see L&C),  l[u] is the
 degree of the elp at that step, and u_l[u] is the difference between the
 step number and the degree of the elp.
 */
/* initialise table entries */

int rs::computeerors()
{
      
    int j;
    d[0] = 0 ;           /* index form */
    d[1] = s[1] ;        /* index form */
    elp[0][0] = 0 ;      /* index form */
    elp[1][0] = 1 ;      /* polynomial form */
    for (int i=1; i<kCodeWordLength-kNumberOfDataBits; i++)
    { 
        elp[0][i] = -1 ;   /* index form */
        elp[1][i] = 0 ;   /* polynomial form */
    }
    l[0] = 0 ;
    l[1] = 0 ;
    u_lu[0] = -1 ;
    u_lu[1] = 0 ;
    int u = 0 ;
    
    do
    {
        u++ ;
        if (d[u]==-1)
        { l[u+1] = l[u] ;
            for (int i=0; i<=l[u]; i++)
            {  elp[u+1][i] = elp[u][i] ;
                elp[u][i] = index_of[elp[u][i]] ;
            }
        }
        else
        /* search for words with greatest u_lu[q] for which d[q]!=0 */
        { 
            int q = u-1 ;
            while ((d[q]==-1) && (q>0)) q-- ;
            /* have found first non-zero d[q]  */
            if (q>0)
            { j=q ;
                do
                { j-- ;
                    if ((d[j]!=-1) && (u_lu[q]<u_lu[j]))
                        q = j ;
                }while (j>0) ;
            } ;
            
            /* have now found q such that d[u]!=0 and u_lu[q] is maximum */
            /* store degree of new elp polynomial */
            if (l[u]>l[q]+u-q)  l[u+1] = l[u] ;
            else  l[u+1] = l[q]+u-q ;
            
            /* form new elp(x) */
            for (int i=0; i<kCodeWordLength-kNumberOfDataBits; i++)    elp[u+1][i] = 0 ;
            for (int i=0; i<=l[q]; i++)
                if (elp[q][i]!=-1)
                    elp[u+1][i+u-q] = alpha_to[(d[u]+kCodeWordLength-d[q]+elp[q][i])%kCodeWordLength] ;
            for (int i=0; i<=l[u]; i++)
            { elp[u+1][i] ^= elp[u][i] ;
                elp[u][i] = index_of[elp[u][i]] ;  /*convert old elp value to index*/
            }
        }
        u_lu[u+1] = u-l[u+1] ;
        
        /* form (u+1)th discrepancy */
        if (u<kCodeWordLength-kNumberOfDataBits)    /* no discrepancy computed on last iteration */
        {
            if (s[u+1]!=-1)
                d[u+1] = alpha_to[s[u+1]] ;
            else
                d[u+1] = 0 ;
            for (int i=1; i<=l[u+1]; i++)
                if ((s[u+1-i]!=-1) && (elp[u+1][i]!=0))
                    d[u+1] ^= alpha_to[(s[u+1-i]+index_of[elp[u+1][i]])%kCodeWordLength] ;
            d[u+1] = index_of[d[u+1]] ;    /* put d[u+1] into index form */
        }
    } while ((u<kCodeWordLength-kNumberOfDataBits) && (l[u+1]<=kNumberOfErrorsToCorrect)) ;
   
    return ++u;
}

int rs::calculateroots(int u)
{
    /* put elp into index form */
    for (int i=0; i<=l[u]; ++i)   elp[u][i] = index_of[elp[u][i]] ;
    /* find roots of the error location polynomial */
    for (int i=1; i<=l[u]; i++)  reg[i] = elp[u][i] ;
    
    int count = 0 ;
    for (int i=1; i<=kCodeWordLength; i++)
    {  
        int q = 1 ;
        for (int j=1; j<=l[u]; j++)
            if (reg[j]!=-1){ 
                reg[j] = (reg[j]+j)%kCodeWordLength ;
                q ^= alpha_to[reg[j]] ;
            } 
        if (!q)        /* store root and error location number indices */
        { 
            root[count] = i;
            loc[count]  = kCodeWordLength-i ;
            count++ ;
        }
    }

    return count;

}
int rs::prepare(std::vector<uint8_t> bindataIn)
{
    size_t bindataLength=bindataIn.size();
    std::vector<int> Bin4Data(kCodeWordLength+2,0);
    int l=0;
    for (int i=0;i<bindataLength; i+=kGFAlpha) {
        int k=0;
        for (int j=i; j<i+kGFAlpha; ++j) {
            Bin4Data[l]+=bindataIn[j]*(1<<(kGFAlpha-1-k++));
        }
        l++;
    }
    std::vector<int> bb(kCodeWordLength+2,0);
    std::vector<int> recd((kCodeWordLength-kNumberOfDataBits)*kGFAlpha,0);
    encode_rs(Bin4Data,bb) ;    
    /* put the transmitted codeword, made up of data plus parity, in recd[] */
   
    for (int i=0; i<kCodeWordLength-kNumberOfDataBits; i++)  
    {
        for (int j=0; j<kGFAlpha; j++) {
            recd[kGFAlpha*i+j]=0x1 & (bb[i]>>(kGFAlpha-1-j));  
            
        } 
           
    }
    int k=(kCodeWordLength-kNumberOfDataBits)*kGFAlpha;
    int iOut=0;
    for (int i=0; i<k;i++)
        iOut+=(recd[i]<<(k-1-i));
   
    return iOut;

}
int rs::Decode(std::vector<int> &recd)
{
    
    for (int i=0; i<kCodeWordLength; i++)
        recd[i]=index_of[recd[i]];
    /* decode recv[] */
   return  decode_rs(recd) ;    

}
    };
};
