#include <iostream>
#include <vector>
#include <GL/glut.h>
using namespace std;
struct State
{
	int n;
	int src;
	int dest;
	int tmp;
	int step;
};

vector<State> stack;
vector<int> rings[3];
int N;
const int win_height = 480;
const int win_width = 852;
const float t_Red = 0.2, t_Green = 0.2, t_Blue = 0.4;


void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.9,0.7,0.6);
	glVertex2i(0,0);
	glColor3f(0.7,0.7,0.8);
	glVertex2i(win_width,0);
	glColor3f(0.7,0.9,0.8);
	glVertex2i(0,win_height);
	glColor3f(0.9,0.7,0.8);
	glVertex2i(win_width,win_height);
	glEnd();
	glBegin(GL_QUADS);
	for (int i = 0; i < 3; ++i)
	{
		const int RingCenter = i * win_width / 3 + win_width / 3 / 2 + 15;
		for (size_t j = 0; j < rings[i].size(); ++j)
		{
			const int RingRadius = (win_width / 3 / 2 - rings[i][j] * win_width / 3 / 2 / N - 5);
			glColor3f(0.1,0.9,0.1);
			glVertex2f(RingCenter - RingRadius - 20, win_height - j * 60);
			glColor3f(t_Red+(rand()%5/50),t_Green+(rand()%5/50),rand()%5/50);
			glVertex2f(RingCenter - RingRadius, win_height - j * 60 - 54);
			glColor3f(t_Red+(rand()%5/50),rand()%5/50,t_Blue+(rand()%5/50));
			glVertex2f(RingCenter + RingRadius - 20, win_height - j * 60 - 54);
			glColor3f((rand()%5)/80,(rand()%5)/80,(rand()%5)/80);
			glVertex2f(RingCenter + RingRadius, win_height - j * 60);
		}
	}
	glEnd();
	glutSwapBuffers();
}

void timer(int = 0)
{
	if (stack.size() > 0)
	{
		State &state = stack.back();
		switch (state.step)
		{
		case 0:
			if (state.n == 0)
				stack.pop_back();
			else
			{
				++state.step;
				State newState;
				newState.n = state.n - 1;
				newState.src = state.src;
				newState.dest = state.tmp;
				newState.tmp = state.dest;
				newState.step = 0;
				stack.push_back(newState);
			}
			break;
		case 1:
			cout << state.src << "->" << state.dest << endl;
			rings[state.dest].push_back(*rings[state.src].rbegin());
			rings[state.src].pop_back();
			++state.step;
			State newState;
			newState.n = state.n - 1;
			newState.src = state.tmp;
			newState.dest = state.dest;
			newState.tmp = state.src;
			newState.step = 0;
			stack.push_back(newState);
			break;
		case 2:
			stack.pop_back();
			break;
		}
		glutPostRedisplay();
		glutTimerFunc(50, timer, 0);
	}
}
void init(int n, int src, int dest, int tmp)
{
	State state;
	state.n = n;
	N=n;
	state.src = src;
	state.dest = dest;
	state.tmp = tmp;
	state.step = 0;
	stack.push_back(state);
	for (int i = 0; i < n; ++i)
	{
		rings[src].push_back(i);
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(win_width, win_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tower of Hanoi");
	glClearColor(0, 0, 0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, win_width, win_height, 0, -1, 1);
	glutDisplayFunc(display);
	init(7, 0, 2, 1);
	timer();
	glutMainLoop();
}

