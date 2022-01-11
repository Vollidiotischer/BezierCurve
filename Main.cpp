#include <SFML/Graphics.hpp> 
#include <iostream>
#include <math.h>
#include <vector>
#include <array>

//Preprocessor Variables
constexpr int screen_x = 1000; 
constexpr int screen_y = 750;


// structs
struct Point {
	int x, y;
	int w = 10, h = 10; 
	Point(int x, int y) {
		this->x = x; 
		this->y = y; 
	}

	Point(int x, int y, int h, int w) {
		this->x = x;
		this->y = y;
		this->w = w; 
		this->h = h; 
	}

	bool hits(int x, int y) {
		return (x > this->x && x < this->x + this->w) && (y > this->y && y < this->y + this->h); 
	}
};

struct Bezier_Curve {
	static const int num_points = 1000; 
	std::vector<Point*> points; 
	std::array<Point*, num_points> drawn_points; 

	Bezier_Curve(std::vector<Point*> points) {
		this->points = points; 
		drawn_points.fill(0); 
	}

	void calculate_path() {

		for (int i = 0; i < drawn_points.size(); i++) {
			delete drawn_points[i];
			
		}
		double step_size = 1.0 / num_points; 

		std::vector<std::vector<Point*>> temp_points(points.size() - 1);
		int temp_index = 0; 

		int draw_points_index = 0; 

		for (double t = 0; t <= 1; t += step_size) {

			temp_index = 0; 

			for (int i = 0; i < points.size()-1; i++) {
				temp_points[temp_index].push_back(calculate_next_point(points[i], points[i+1], t)); 
			}


			while (temp_index < temp_points.size()-1) {
				for (int i = 0; i < temp_points[temp_index].size() - 1; i++) {

					temp_points[temp_index + 1].push_back(calculate_next_point(temp_points[temp_index][i], temp_points[temp_index][i + 1], t));
				}
				temp_index++; 
			}

			drawn_points[draw_points_index] = temp_points[temp_index][0];
			draw_points_index++;

			for (int i = 0; i < temp_points.size(); i++) {

				if (i < temp_points.size() - 1) {
					for (int i2 = 0; i2 < temp_points[i].size(); i2++) {
						delete temp_points[i][i2];
					}
				}

				temp_points[i].clear();
			}			


		}

	}

	Point* calculate_next_point(Point* A, Point* B, double t) {
		Point *C = new Point(0, 0); 

		C->x = A->x + t * ((double)B->x - A->x); 
		C->y = A->y + t * ((double)B->y - A->y);

		return C; 
	}
};

// Function Promises
void init();
void draw(Bezier_Curve*); 
void events(Bezier_Curve*);

// Variables
sf::RenderWindow* window; 

int main() {

	init(); 


	Point *A = new Point(0, 600);
	Point *B = new Point(screen_x - 10, 0);
	Point *C = new Point(screen_x-10, screen_y - 10);
	Point *D = new Point(0, 400);


	Bezier_Curve* bc = new Bezier_Curve({ A, B, C, D }); 

	bc->calculate_path(); 

	bool button_is_pressed = false; 
	int state = 0;


	while (window->isOpen()) {

		events(bc); 
		

		draw(bc); 

		
	}

	

	return 0; 
}

void init() {
	window = new sf::RenderWindow(sf::VideoMode(screen_x, screen_y), "Bezier Curve"); 

	window->setKeyRepeatEnabled(false); 

}

void events(Bezier_Curve* bc) {

	static bool button_is_pressed = false; 
	static int state = 0; 

	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window->close();
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			int mox = sf::Mouse::getPosition(*window).x;
			int moy = sf::Mouse::getPosition(*window).y;

			for (int i = 0; i < bc->points.size(); i++) {
				if (bc->points[i]->hits(mox, moy)) {

					button_is_pressed = true;
					state = i;
					break;
				}
			}
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			button_is_pressed = false;
		}
	}

	if (button_is_pressed) {
		int new_x = sf::Mouse::getPosition(*window).x;
		int new_y = sf::Mouse::getPosition(*window).y;

		bc->points[state]->x = new_x;
		bc->points[state]->y = new_y;

		bc->calculate_path();
	}
}

void draw(Bezier_Curve* bc) {
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