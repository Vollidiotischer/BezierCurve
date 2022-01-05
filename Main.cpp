#include <SFML/Graphics.hpp> 
#include <iostream>
#include <math.h>
#include <vector>
#include <array>

//Preprocessor Variables
constexpr int screen_x = 1000; 
constexpr int screen_y = 750;

// Function Promises
void init(); 



// structs
struct Point {
	int x, y; 
	Point(int x, int y) {
		this->x = x; 
		this->y = y; 
	}
};

/*
quadratic: 
 0 - 1 = 3
 1 - 2 = 4

 3 - 4 = 5 

 5 = point

 cubic: 
 0 - 1 = 4
 1 - 2 = 5
 2 - 3 = 6

 4 - 5 = 7
 5 - 6 = 8

 7 - 8 = 9

 9 = point


*/
struct Bezier_Curve {
	double t = 0; 
	std::vector<Point*> points; 
	std::vector<Point*> drawn_points; 

	Bezier_Curve(std::vector<Point*> points) {
		this->points = points; 
	}

	void calculate_path() {

		drawn_points.clear(); 

		std::vector<std::vector<Point*>> temp_points(points.size() - 1);
		int temp_index = 0; 

		for (t = 0; t <= 1; t += 0.01) {

			temp_index = 0; 
			
			for (int i = 0; i < temp_points.size(); i++) {
				temp_points[i].clear(); 
			}

			for (int i = 0; i < points.size()-1; i++) {
				temp_points[temp_index].push_back(calculate_next_point(points[i], points[i+1], t)); 
			}


			while (temp_index < temp_points.size()-1) {
				for (int i = 0; i < temp_points[temp_index].size() - 1; i++) {

					temp_points[temp_index + 1].push_back(calculate_next_point(temp_points[temp_index][i], temp_points[temp_index][i + 1], t));
				}
				temp_index++; 
			}

			drawn_points.push_back(temp_points[temp_index][0]); 
		}

	}

	Point* calculate_next_point(Point* A, Point* B, double t) {
		Point *C = new Point(0, 0); 

		C->x = A->x + t * ((double)B->x - A->x); 
		C->y = A->y + t * ((double)B->y - A->y);

		return C; 
	}
};

// Variables
sf::RenderWindow* window; 

int main() {

	init(); 

	int state = 0; 

	Point *A = new Point(0, 600);
	Point *B = new Point(screen_x - 10, 0);
	Point *C = new Point(screen_x-10, screen_y - 10);
	Point *D = new Point(0, 400);

	Bezier_Curve* bc = new Bezier_Curve({ A, B, C, D }); 

	bc->calculate_path(); 

	while (window->isOpen()) {

		sf::Event event; 
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close(); 
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Num1) {
					state = 0; 
				}
				if (event.key.code == sf::Keyboard::Num2) {
					state = 1; 
				}
				if (event.key.code == sf::Keyboard::Num3) {
					state = 2; 
				}
				if (event.key.code == sf::Keyboard::Num4) {
					state = 3; 
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				int mox = sf::Mouse::getPosition(*window).x;
				int moy = sf::Mouse::getPosition(*window).y;

				bc->points[state]->x = mox; 
				bc->points[state]->y = moy; 
				
				bc->calculate_path(); 
			}
		}



		window->clear(sf::Color::White);

		sf::RectangleShape pixel({ 2, 2 });

		pixel.setFillColor(sf::Color::Black);

		for (int i = 0; i < bc->drawn_points.size(); i++) {
			pixel.setPosition({ (float)bc->drawn_points[i]->x, (float)bc->drawn_points[i]->y });
			window->draw(pixel);

		}

		pixel.setSize({ 10, 10 }); 

		pixel.setFillColor(sf::Color::Red); 
		for (int i = 0; i < bc->points.size(); i++) {
			pixel.setPosition({ (float)bc->points[i]->x, (float)bc->points[i]->y }); 
			window->draw(pixel); 
		}

		window->display();
	}

	

	return 0; 
}

void init() {
	window = new sf::RenderWindow(sf::VideoMode(screen_x, screen_y), "Bezier Curve"); 

	window->setKeyRepeatEnabled(false); 

}
