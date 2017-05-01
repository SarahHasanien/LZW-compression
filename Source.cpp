#include <iostream>
#include <fstream>
#include <string>
#include <utility>
using namespace std;
//Functions related to encoding part
bool foundInEncDic(string search,string file)
{
	ifstream myfile;
	myfile.open(file);
	int id; 
	string s;

	while (myfile >> id >> s)
	{
		if (s == search)
			return true;
	}
	return false;
}
void addToDic(string s, int j,string file)
{
	ofstream out;
	out.open(file,std::ios_base::app); //Append to insert into file without deleting the previous data =D
	out << j << " " << s << endl;
	out.close();
}
void addToEncodingFile(string myS,string dicFile,string encodingFile)
{
	//Out the id of this string to the encoding file
	ifstream myfile;
	myfile.open(dicFile);
	int id;
	string s;
	while (myfile >> id >> s)
	{
		if (s == myS)
		{
			ofstream out;
			out.open(encodingFile, std::ios_base::app); //Append to insert into file without deleting the previous data =D
			out << id <<' ';
			out.close();
			return;
		}
	}
}
//Functions related to decoding part
string getStirngOfIndx(int indx, string file)
{
	ifstream myfile;
	myfile.open(file);
	int id;
	string s;
	while (myfile >> id >> s)
	{
		if (id==indx)
		{
			return s;
		}
	}
	return "";
}
//Coparison function to be called at the end of the main
bool compare(string file1, string file2)
{
	string s1, s2; char c;
	ifstream in1(file1), in2(file2);

	in1 >> s1;
	in2 >> s2;
	in1.close();
	in2.close();
	if (s1 == s2)
		return true;
	else
		return false;

}
int main()
{
	
	//Open text file which contains the original message
	string inputMsg = "inputMessage.txt";
	string encoderDic = "Encoder_Dictionary.txt";
	string decoderDic = "Decoder_Dictionary.txt";
	string encFile = "Encoding.txt";
	string decFile = "Decoding.txt";

	/*The next 3 lines are written to open the encoding file without appending to clear the data written in it in
	previous attempts*/
	ofstream out;
	out.open(encFile);
	out.close();

	//Genereate initial encoder dictionary
	ofstream ofs;
	ofs.open(encoderDic);
	int j = 0;
	for (char i = 'a'; i <= 'z'; i++)
	{
		ofs << j<<" "<<i<<endl;
		j++;
	}
	ofs << j << " " << '_' << endl; //space
	j++;
	ofs.close();
	//End Genereating initial encoder dictionary

	//Open original msg file and the file which will contain the dictionary(encoder)
	char c;
	string s = "";
	ifstream inMsg;
	inMsg.open(inputMsg);
	//LZW encoding is applied here:
	while (inMsg.get(c))
	{
		//Pesuodo: if dectionary contains s+c: s+=c else encode s
		if (foundInEncDic(s + c, encoderDic))
		{
			s =s+ c;
		}
		else
		{
			addToEncodingFile(s, encoderDic,encFile);
			addToDic(s + c, j, encoderDic);
			j++;
			s = c;
		}
	}
	inMsg.close();
	addToEncodingFile(s, encoderDic, encFile);
	//End LZW encoding

	//Genereate initial decoder dictionary
	ofstream of;
	of.open(decoderDic);
	int k = 0;
	for (char i = 'a'; i <= 'z'; i++)
	{
		of << k << " " << i << endl;
		k++;
	}
	of << k << " " << '_' << endl; //space
	k++;
	of.close();
	//End Genereating initial decoder dictionary
	encFile = "Encoding.txt";
	decFile = "Decoding.txt";
	//LZW decoding is applied here
	string prev = "", entry ;
	int indx;
	ifstream encMsg(encFile, std::ios_base::in);
	ofstream ofdecoded(decFile);
	
	while (encMsg >> indx)
	{
		entry = getStirngOfIndx(indx, decoderDic);
		if (entry == "")
			entry = prev + prev[0];
		ofdecoded << entry;

		if (prev != "")
		{
			addToDic(prev + entry[0], k, decoderDic);
			k++;
		}
		prev = entry;
	}
	encMsg.close();
	ofdecoded.close();
	//End LZW decoding

	//Call comparison function here
	if (compare(inputMsg, decFile))
		cout << "Data decoded successfully!";
	else
		cout << "Original data and decoded data are not matched!!\n";


	cout << "\n\n\n\nKindly read the attached ReadMe file =D\n\n\n";
}
