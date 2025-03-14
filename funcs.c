#include "funcs.h"

void	node_data(Node* node)
{
	printf("node inputs :\n");
	for (size_t i = 0; i < node->n_inputs; ++i)
	{
		printf("%f -", node->input[i]);
	}
	printf("\nnode weights :\n");
	for (size_t i = 0; i < node->n_inputs; ++i)
	{
		printf("%f -", node->weight[i]);
	}
	printf("\nnode output =  %f\n---------------------\n", node->output);
}

void	layer_data(Layer* l)
{
	for (size_t i = 0; i < l->n_nodes; ++i)
	{
		node_data(l->nodes[i]);
	}
}

//--------------------------------

double relu(double n)
{
	return (n <= 0 ? 0 : n);
}

double sigmoid(double n) // for binary output
{
	return (1 / (1 + pow(EULER_NUMBER, -n)));
}

double	xavier_init(size_t n_inputs)
{
	return ((double)rand() / RAND_MAX * 2.0 - 1.0) * sqrt(1.0 / n_inputs);
}

//------------------------------------

Node*	init_node(size_t n_inputs, double bias)
{
	Node* res = gb_malloc(1, sizeof(Node), ALLOC);
	res->bias = bias;
	res->n_inputs = n_inputs;
	res->input = gb_malloc(n_inputs, sizeof(double), ALLOC);
	res->weight = gb_malloc(n_inputs, sizeof(double), ALLOC);
	res->output = 0;

	for (size_t i = 0; i < n_inputs; ++i)
	{
		res->weight[i] = xavier_init(n_inputs);
	}

	return (res);
}

Layer*	init_layer(layer_type t, size_t n_nodes, size_t prev_n_nodes)
{
	Layer* res = gb_malloc(1, sizeof(Layer), ALLOC);
	res->nodes = gb_malloc(n_nodes + 1, sizeof(Node*), ALLOC);
	res->n_nodes = n_nodes;
	res->t = t;
	res->next = NULL;
	res->back = NULL;

	for (size_t i = 0; i < res->n_nodes; ++i)
	{
		res->nodes[i] = init_node(prev_n_nodes, BIAS);
	}

	return (res);
}

NN*	init_nn(size_t n_layers,
			size_t n_input, size_t n_hidden, size_t n_output,
			void* hidden_activ, void* output_activ)
{
	NN* res = gb_malloc(1, sizeof(NN), ALLOC);
	res->n_layers = n_layers;
	res->layers = gb_malloc(n_layers + 1, sizeof(Layer*), ALLOC);

	for (size_t i = 0; i < n_layers; ++i)
	{
		if (i == 0) // input
		{
			res->layers[i] = init_layer(INPUT, n_input, 1);
			res->layers[i]->layer_activ = NULL;
		}
		else if (i == n_layers - 1) // output
		{
			res->layers[i] = init_layer(OUTPUT, n_output, n_hidden);
			res->layers[i]->layer_activ = output_activ;
		}
		else // hidden
		{
			if (i - 1 == 0)
				res->layers[i] = init_layer(HIDDEN, n_hidden, n_input);
			else
				res->layers[i] = init_layer(HIDDEN, n_hidden, n_hidden);
			res->layers[i]->layer_activ = hidden_activ;
		}
	}

	// init back and forth pointers between layers

	for (size_t i = 0; i < n_layers - 1; ++i)
		res->layers[i]->next = res->layers[i + 1];
	for (size_t i = n_layers - 1; i > 0; --i)
		res->layers[i]->back = res->layers[i - 1];

	// set usefull pointers

	res->input_layer = res->layers[0];
	res->output_layer = res->layers[n_layers - 1];

	return (res);
}

//--------------------------------

void take_input(Layer* input_layer, double* sample)
{
    for (size_t i = 0; i < input_layer->n_nodes; ++i)
	{
        Node* cur_node = input_layer->nodes[i];
        cur_node->input[0] = sample[i];
    }
}

void	collect_output(Layer* l1, Layer* l2)
{
	for (size_t i = 0; i < l2->n_nodes; ++i)
	{
		Node* curr_node = l2->nodes[i];
		for (size_t j = 0; j < l1->n_nodes; ++j)
		{
			curr_node->input[j] = l1->nodes[j]->output;
		}
	}
}

void	compute_node(Node* n, double (*activ)(double))
{
	double res = 0;

	for (size_t i = 0; i < n->n_inputs; ++i)
	{
		res += n->weight[i] * n->input[i];
	}
	res += n->bias;
	if (!activ)
		n->output = activ(res);
	else
		n->output = res;
}

void compute_layer(Layer* l)
{
    if (l->t == INPUT)
	{
        for (size_t i = 0; i < l->n_nodes; ++i)
		{
            Node* node = l->nodes[i];
            node->output = node->input[0];
        }
    }
	else
	{
        collect_output(l->back, l);
		void*	activ = NULL;
		switch (l->t)
		{
			case OUTPUT:
				activ = sigmoid;
				break ;
			case HIDDEN:
				activ = relu;
				break ;
			case INPUT:
				activ = NULL;
				break ;
		}
        for (size_t i = 0; i < l->n_nodes; ++i)
		{
            compute_node(l->nodes[i], activ);
        }
    }
}

void	forward_propagation(NN* nn, double* data)
{
	if (nn->n_layers < 3)
	{
		fprintf(stderr, "need at least 3 layers\n");
		exit(1);
	}
	take_input(nn->input_layer, data);
	for (size_t i = 0; i < nn->n_layers; ++i)
	{
		compute_layer(nn->layers[i]);
	}
}