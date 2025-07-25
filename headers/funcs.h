#ifndef FUNCS_H
#define FUNCS_H

#include "types.h"
#include "macro.h"
#include "garbage.h"



Node*	init_node(size_t n_inputs, double bias);
Layer*	init_layer(layer_type t, size_t n_nodes, size_t prev_n_nodes);
NN*		init_nn(size_t n_layers,
				size_t n_input, size_t n_hidden, size_t n_output,
				void* hidden_activ, void* output_activ);

void 	take_input(Layer* input_layer, double* sample);
void	collect_output(Layer* l1, Layer* l2);
void	compute_node(Node* n, double (*activ)(double));
void	compute_layer(Layer* l);
void	forward_propagation(NN* nn, double* data);

double	relu(double n);
double	sigmoid(double n);
double	sigmoid_derivative(double n);
double	xavier_init(size_t n_inputs);

void	node_data(Node* node);
void	layer_data(Layer* l);

double	mean_squared_error(NN* nn, double* true_data);
void	calculate_delta(NN* nn, double* true_data);
void	back_prop_hidden_layers(NN* nn);
void	update_weights_biases(NN* nn, double learning_rate);
void	back_propagate(NN* nn, double* true_data);

#endif
