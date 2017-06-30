#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include "Automata.h"

#ifdef _WIN32
#define CLEAR "cls"
#else //For linux
#define CLEAR "clear"
#endif

int genericMenu(std::vector<std::string> options, std::string title) {

	int curr_option = 0;

	system(CLEAR);

	while (true) {
		std::cout << "SIDIH 2016/2017" << std::endl << "by Jose Silva (1130352) & Daniel Freire (1130858)\n\n\n";

		std::cout << title << "\n\n";

		for (int i = 0; i != options.size(); i++) {
			std::cout << i + 1 << ") " << options.at(i) << std::endl;
		}

		std::cout << std::endl << "Choose option: ";
		std::cin >> curr_option;

		if (std::cin.fail() || curr_option < 1 || curr_option > options.size()) {
			std::cin.clear();
			std::cin.ignore(256, '\n');
			system(CLEAR);
			continue;
		}

		//Delete extra \n in cin buffer
		std::cin.get();

		curr_option--;

		system(CLEAR);

		return curr_option;
	}

}

void automataMenu(std::vector<Automata>& automata_vect, std::vector<std::string>& automata_names) {
	
	std::vector<std::string> all_options = automata_names;
	
	all_options.push_back("Return to main menu");
	int automaton_index = genericMenu(all_options, "Select automaton");
	
	while (true) {
		if (automaton_index == automata_names.size()) {
			return;
		}
		else if (automaton_index < automata_names.size()) {
			auto option = 0;
			std::string title;
			title = "Automaton " + automata_names.at(automaton_index);
			option = genericMenu({"Check automaton type",
								  "Remove non-accessible states",
								  "Remove non-coaccessible states",
								  "Trim",
								  "Convert to DFA",
								  "Compute sync product with other automaton",
								  "Compute async product with other automaton",
								  "Display automaton",
								  "Rename automaton",
								  "Delete automaton",
								  "Save automaton to File",
								  "Return to main menu"
			}, title);

			switch (option) {
				case 0:
					automata_vect.at(automaton_index).isDFA(std::cout);
					std::cout << std::endl << "Press enter to continue";
					std::cin.ignore();
					break;

				case 1:
					automata_vect.at(automaton_index).removeNonAccessibleStates(std::cout);
					std::cout << std::endl << "Press enter to continue";
					std::cin.get();
					break;

				case 2:
					automata_vect.at(automaton_index).removeNonCoaccessibleStates(std::cout);
					std::cout << std::endl << "Press enter to continue";
					std::cin.get();
					break;

				case 3:
					automata_vect.at(automaton_index).trim(std::cout);
					std::cout << std::endl << "Press enter to continue";
					std::cin.get();
					break;

				case 4:
					automata_vect.at(automaton_index).toDFA(std::cout);
					std::cout << std::endl << "Press enter to continue";
					std::cin.get();
					break;

				case 5:
				case 6:
					{

						if (automata_vect.at(automaton_index).isDFA() == false) {
							std::cout << "Both Automata must be deterministic in order to compute the product" << std::endl;
							std::cout << "Press enter to continue";
							std::cin.get();
							break;
						}

						std::vector<std::string> product_options = automata_names;
						product_options.push_back("Cancel");
						int product_automaton_index = genericMenu(product_options, "Select Automaton to compute product with");

						while (true) {
							if (product_automaton_index == automata_names.size()) {
								break;
							}
							else if (product_automaton_index < automata_names.size()) {
								if (automata_vect.at(product_automaton_index).isDFA() == false) {
									std::cout << "Both Automata must be deterministic in order to compute the product" << std::endl;
									std::cout << "Press enter to continue";
									std::cin.get();
									break;
								}
								
								Automata product_result;
								std::string product_result_name;

								if (option == 5) {
									//Sync product	
									product_result = automata_vect.at(automaton_index) * automata_vect.at(product_automaton_index);
									product_result_name = automata_names.at(automaton_index) + " x " + automata_names.at(product_automaton_index);
								}
								else {
									//Async product	
									product_result = automata_vect.at(automaton_index) + automata_vect.at(product_automaton_index);
									product_result_name = automata_names.at(automaton_index) + " || " + automata_names.at(product_automaton_index);
								}

								if (product_result.automataHasData() == false) {
									std::cout << "Error: product generates an invalid automaton" << std::endl;
								}
								else {

									auto it = std::find(automata_names.begin(), automata_names.end(), product_result_name);
									if (it != automata_names.end()) {
										auto i = 2;
										std::string new_name;
										while (it != automata_names.end()) {
											new_name = product_result_name + " (" + std::to_string(i) + ")";
											it = std::find(automata_names.begin(), automata_names.end(), new_name);
											i++;
										}
										product_result_name = new_name;
									}

									std::string input;
									std::cout << "Enter automaton name [press enter for default: " << product_result_name << "]:" << std::endl;
									std::getline(std::cin, input);
									if (input.empty() == true) {
										automata_names.push_back(product_result_name);
										automata_vect.push_back(product_result);
									}
									else {

										it = std::find(automata_names.begin(), automata_names.end(), input);
										if (it != automata_names.end()) {
											std::cout << "Automaton " << input << " already exists. Overwrite? (y/n)" << std::endl;
											std::string answer;
											std::cin >> answer;
											if (answer == "y" || answer == "Y" || answer == "Yes" || answer == "yes") {
												automata_vect[std::distance(automata_names.begin(), it)] = product_result;
											}
										}
										else {
											automata_names.push_back(input);
											automata_vect.push_back(product_result);
										}
									}
									
								}
								break;
							}
						}
					}
					break;

				case 7:
					automata_vect.at(automaton_index).printAutomataInfo(std::cout);
					std::cout << std::endl << "Press enter to continue";
					std::cin.get();
					break;

				case 8:
					{
						std::string new_name;
						std::cout << "Enter new automaton name [press enter to keep name]:" << std::endl;
						std::getline(std::cin, new_name);
						if (new_name.empty() == false) {
							auto it = std::find(automata_names.begin(), automata_names.end(), new_name);
							if (it != automata_names.end()) {
								std::cout << "Automaton " << new_name << " already exists." << std::endl;
							}
							else {
								automata_names[automaton_index] = new_name;
							}
						}
						else {
							break;
						}
						std::cout << std::endl << "Press enter to continue";
						std::cin.get();
					}
					break;

				case 9:
					{
						std::cout << "Are you sure you want to delete the automaton " << automata_names.at(automaton_index) << " from memory? (y/n)" << std::endl;
						std::string answer;
						std::cin >> answer;
						std::cin.get();
						if (answer == "y" || answer == "Y" || answer == "Yes" || answer == "yes") {
							automata_names.erase(automata_names.begin() + automaton_index);
							automata_vect.erase(automata_vect.begin() + automaton_index);
							std::cout << std::endl << "Automaton deleted";
							std::cout << std::endl << "Press enter to continue";
							std::cin.get();
							return;
						}
					}
					std::cout << std::endl << "Press enter to continue";
					std::cin.get();
					break;

				case 10:
					{
						std::string path, filename;
						std::cout << "Enter folder to save to (full or relative path):" << std::endl;
						std::cin >> path;
						std::cin.get();
						std::cout << "Enter file name (.aut will be added to file name):" << std::endl;
						std::cin >> filename;
						std::cin.get();
						automata_vect.at(automaton_index).saveToFile(path + "//" + filename + ".aut", std::cout);
						std::cout << std::endl << "Press enter to continue";
						std::cin.get();
					}
					break;

				case 11:
					return;
					break;

				default:
					break;
			}
		}
	}
}

int main() {

	std::vector<Automata> automata_vect;
	std::vector<std::string> automata_names;
	
	while (true) {

		int option = genericMenu({
			"Load automaton from file",
			"Select automata in memory",
			"Exit"
			}, "Main Menu");

		switch (option) {

		case 0:
			{
				Automata new_aut;
				std::string input;
				std::cout << "Enter file path to load (use full path or filename for relative path):" << std::endl;
				std::cin >> input;
				std::cin.get();
				new_aut.loadFromFile(input, std::cout);
				if (new_aut.automataHasData() == true) {
					std::cout << std::endl << "Enter automaton name:" << std::endl;
					std::cin >> input;
					auto it = std::find(automata_names.begin(), automata_names.end(), input);
					if ( it != automata_names.end()) {
						std::cout << "Automaton " << input << " already exists. Overwrite? (y/n)" << std::endl;
						std::string answer;
						std::cin >> answer;
						if (answer == "y" || answer == "Y" || answer == "Yes" || answer == "yes") {
							automata_vect[ std::distance(automata_names.begin(), it) ] = new_aut;
						}
					}
					else {
						automata_vect.push_back(new_aut);
						automata_names.push_back(input);
					}
				}
				std::cin.get();
			}
			break;

		case 1:
			automataMenu(automata_vect, automata_names);
			break;

		case 2:
			return 0;
			break;

		default:
			break;
		}
	}
	
	return 0;
}