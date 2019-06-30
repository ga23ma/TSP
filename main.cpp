#include "main.h"

int main(int argc, char *argv[]){
    // read parameters from command line
    Params params;
    readParams(params, argc, argv);

    // read instance from file
    Instance instance;
    readInstance(params.inputFile, instance);

    // solve it
    auto started = chrono::high_resolution_clock::now();
	vector<int> solution;
	
	if(params.algorithm.compare("A") == 0){
		solution = solveBottomUp(instance, params.timelimit, started);
	}
	else if(params.algorithm.compare("B") == 0){
		solution = solveTopDown(instance, params.timelimit, started);
	}
	
	// output data
    auto done = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(done-started).count();
    //cout << "time to solve: " << time << " ms" << endl;
    
	printSolution(instance, solution);
	
	if(params.showGraph){
		createPDF(instance, solution);
	}
	
    return 0;
}

// read argv params
void readParams(Params &params, int argc, char *argv[])
{
    params.algorithm = "";
    params.inputFile = "";
    params.showGraph = false;
    params.timelimit = 60;

    // Read
    for(int i = 1; i < argc; i++){
        const string arg(argv[i]);
        string next;

        if((i+1) < argc){
            next = string(argv[i+1]);
        }
        else{
            next = string("");
        }

        if(arg.find("-i") == 0 && next.size() > 0){
            params.inputFile = next;
            i++;
            continue;
        }

        if(arg.find("-a") == 0 && next.size() > 0){
            params.algorithm = next;
            i++;
            continue;
        }

        if(arg.find("-g") == 0){
            params.showGraph = true;
            continue;
        }

        if(arg.find("-t") == 0){
            params.timelimit = stoi(next);
            continue;
        }
        
        cerr << "Invalid parameter." << endl;
        exit(1);
    }

    // Check
    if(params.inputFile == "" || params.algorithm == ""){
        cerr << "Input file and algorithm should be specified" << endl;
        exit(1);
    }
}

// read file and get the corresponding instance
void readInstance(string filename, Instance &instance){
	instance.instanceName = filename;
	ifstream file(filename);
	
	if (file.is_open()) {
		string line;
		int i = 0;
		
		while (getline(file, line)) {
			if(i == 0){
				instance.n = stoi(line);
			}
			else{
				istringstream ss(line); 
				string aux1, aux2;
				ss >> aux1;
				ss >> aux2;
				
				Point p;
				p.x = stod(aux1);
				p.y = stod(aux2);
				instance.points.push_back(p);
			}
			
			i++;
		}
		
		file.close();
	}
	else {
		cout << "Error opening instance file" << endl;
		exit(1);
	}
}

// this will create a pdf file with a visualization of the solution
void createPDF(Instance &instance, vector<int> &solution){
    string tempname = ".viewgraphtemp";
    string outputname = "out.pdf";

    ofstream myfile;
    myfile.open(tempname);
	
	// get bounding box coordinates
	double minpx=DBL_MAX,minpy=DBL_MAX,maxpx=-DBL_MAX,maxpy=-DBL_MAX,delta,factor;
	
	for (auto p : instance.points) {
		if(p.x < minpx)
			minpx = p.x;
			
		if(p.x > maxpx)
			maxpx = p.x;
			
		if(p.y < minpy)
			minpy = p.y;
		if(p.y > maxpy)
			maxpy = p.y;
	}
	
	factor = 20.0; // quanto maior, menor o desenho do vértice
	delta = fmax(maxpx - minpx, maxpy - minpy);
	
	// generate a text file with the graph format of neato program
	myfile << "digraph g {\n";
	myfile << "\tnode [\n";
	myfile << "shape = \"circle\",\n";
	myfile << "style = \"bold\",\n";
	myfile << "color = \"black\",\n";
	myfile << "fontsize = " << 20 << ",\n";
	myfile << "];\n";

	for(int i = 0; i < instance.points.size(); i++) {
		if(i == 0 || i == instance.points.size() - 1){
			myfile << "\t" << "\"" << to_string(i).c_str() << "\" [style = \"bold\", color=\"" << "RED" << "\""
				<< "pos = \"" << factor*(instance.points[i].x - minpx)/delta << "," << factor*(instance.points[i].y-minpy)/delta << "!\" ];\n";	
		}
		else{
			myfile << "\t" << "\"" << to_string(i).c_str() << "\" [style = \"bold\", color=\"" << "BLACK" << "\""
				<< "pos = \"" << factor*(instance.points[i].x - minpx)/delta << "," << factor*(instance.points[i].y-minpy)/delta << "!\" ];\n";	
		}
	}

	for(int i = 0; i < solution.size(); i++){
		if(i == 0){
			myfile << "\t\"0\" -> \"" << to_string(solution[i]).c_str() << "\"" << " [color=\"" << "BLACK" << "\" penwidth=\"1.5\"];\n";
		}
		
		if(i == solution.size() - 1){
			myfile << "\t\"" << to_string(solution[i]).c_str() << "\" -> \"" << instance.n - 1 << "\"" << " [color=\"" << "BLACK" << "\" penwidth=\"1.5\"];\n";
		}
		
		else{
			myfile << "\t\"" << to_string(solution[i]).c_str() << "\" -> \"" << to_string(solution[i + 1]).c_str() << "\"" << " [color=\"" << "BLACK" << "\" penwidth=\"1.5\"];\n";
		}
	}

	myfile << "label=\"solution: " << instance.instanceName << "\";\n";
	myfile << "fontsize=" << 20 << ";\n";
	myfile << "}\n";
	myfile.close();

	// create pdf file using neato
	string cmd = "neato -Tpdf " + tempname + " -o " + outputname;
	system(cmd.c_str());
}

// print solution
void printSolution(Instance &instance, vector<int> &solution){
	/*
	cout << "0 -> ";
	for(auto x : solution){
		cout << x << " -> ";
	}
	cout << instance.n - 1 << endl;
	*/
	
	double dist = 0;
	
	cout << "Distance:" << endl;
	
	for(int i = 0; i < solution.size(); i++){
		if(i == 0){
			dist += getDistance(instance.points[0], instance.points[solution[i]]);
		}
		else if (i == solution.size() - 1){
			dist += getDistance(instance.points[solution[i - 1]], instance.points[solution[i]]);
			dist += getDistance(instance.points[solution[i]], instance.points[instance.n - 1]);
		}
		else{
			dist += getDistance(instance.points[solution[i - 1]], instance.points[solution[i]]);
		}
	}
	
	cout << dist << endl;
}
