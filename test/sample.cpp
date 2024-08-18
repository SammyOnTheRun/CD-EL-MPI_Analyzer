#include <mpi.h>
#include <stdio.h>
void gatherData(int* sendbuf, int* recvbuf, int count, int root, MPI_Comm comm) {
MPI_Status status;
int rank;
MPI_Comm_rank(comm, &rank);
if (rank == root) {
for (int i = 0; i < count; ++i) {
recvbuf[i] = 0;
}
}
MPI_Sendrecv(sendbuf, count, MPI_INT, root, 0, recvbuf, count, MPI_INT, root,
0, comm, &status);
}
void distributeData(int* sendbuf, int* recvbuf, int count, int root, MPI_Comm
comm) {
MPI_Status status;
int rank;
MPI_Comm_rank(comm, &rank);
if (rank == root) {
for (int i = 0; i < count; ++i) {
recvbuf[i] = sendbuf[i];
}
}
MPI_Sendrecv(sendbuf, count, MPI_INT, root, 0, recvbuf, count, MPI_INT, root,
0, comm, &status);
}
int main(int argc, char** argv) {
MPI_Init(&argc, &argv);
int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
int data = rank;
int gatheredData[4] = {0};
gatherData(&data, gatheredData, 1, 0, MPI_COMM_WORLD);
int sendbuf[4] = {1, 2, 3, 4};
int recvbuf[4] = {0};
distributeData(sendbuf, recvbuf, 4, 0, MPI_COMM_WORLD);
MPI_Finalize();
return 0;
}
