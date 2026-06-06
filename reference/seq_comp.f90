program sequence_comparison

! Versione 6 (febbraio 2015)
! Modifiche:
! - Nel caso di un numero di domini troppo grande, invece di calcolare
! tutte le permutazioni possibili si limita a fare una ricerca approssimativa
! tenendo conto delle sovrapposizioni con un numero maggiore di atomi
! - I file in input ora hanno il formato dell'output di SPECTRUS, percio'
! presentano due colonne, una con l'indice (che viene scartato) e l'altra con
! le labels. I file in output con le labels allineate, invece, restano a 
! singola colonna


! Versione 5 (febbraio 2015)
! Modifiche:
! - aggiunta la subroutine fast_seq_intersection (le intersezioni
! tra le labels delle due sequenze sono calcolate once-for-all all'inizio e 
! immagazzinate in una matrice)
! - aggiunto un sequences swap nel caso in cui la seconda sequenza non abbia 
! un numero di domini maggiore o uguale della prima 

! Versione 4 (luglio 2014)
! Modifiche:
! - la seconda sequenza viene allineata sulla prima

! Versione 3.
! Modifiche:
! - i nomi dei file vengono letti da riga di comando
! - i file contengono solo la colonna delle labels

implicit none

integer, parameter :: N_STEP_MAX = 10**9
integer, parameter :: N_ATOMS_MAX = 10**5
integer, parameter :: N_DOMAINS_ROUGH_APPROACH = 10 

! double precision :: pos_atom(1000,3)
! character :: a
! double precision :: tmp
integer :: label1(N_ATOMS_MAX), label2(N_ATOMS_MAX)
integer :: label2m(N_ATOMS_MAX),label2_aligned(N_ATOMS_MAX)
integer, allocatable :: map(:),map2(:)
integer, allocatable :: M(:,:)
integer, allocatable :: order(:), nn(:)
real, allocatable :: nn_value(:), nn_value_old(:)
integer :: i,j,n,k,i_min,n_atoms,n_domains,n_domains2
logical nextp
external nextp
real ratio, max_ratio, ratio_old
CHARACTER(LEN=100) :: file1, file2
integer :: narg
integer :: swap_flag = 0
integer :: relabel_flag, i1, i2




narg = IARGC()
IF (narg .ne. 2) THEN
  write(*,*) "# ERROR: two double-column files must be provided!"
  write(*,*) "# Files must contain a column with indexes (not used) and one with labels."
  stop 1
ELSE
  CALL GETARG(1,file1)
  ! Grab the first command line argument
  ! and store it in the variable 'file1'
  CALL GETARG(2,file2) 
  !Grab the 2nd command line argument
ENDIF
open(81,file=file1,status='old')
open(82,file=file2,status='old')


! !(for reading directly from *.pdb file)
!
! character(LEN=50) :: format = '(A5,I6,2X,A2,2X,A3,I6,4X,3F8.3,2F6.2,11X,A1)'
! read(81,*,END=91) a,j,a,a,j,pos_atom(i,1),pos_atom(i,2),pos_atom(i,3),tmp,tmp


! read input files (labels in the second column)
label1 = -1 
i_min = 100000
k = 0
do i=1,N_ATOMS_MAX
  read(81,*,END=91) j, label1(i)
  if(label1(i).lt.i_min) i_min = label1(i)
  k = k + 1
end do
write(*,*) "# ERROR: Too many atoms!"
stop 1
91 continue
n_atoms = k
if(i_min.lt.0) then
  write(*,*) "# ERROR: only positive labels are allowed!"
  write(*,*) "# check file ", file1
  stop 1
end if
if(i_min.eq.0) label1 = label1 + 1

label2 = -1 
i_min = 100000
k = 0
do i=1,N_ATOMS_MAX
  read(82,*,END=92) j, label2(i)
  if(label2(i).lt.i_min) i_min = label2(i)
  k = k + 1
end do
write(*,*) "# ERROR: Too many atoms!"
stop 1
92 continue
if(i_min.lt.0) then
  write(*,*) "# ERROR: only positive labels are allowed!"
  write(*,*) "# check file ", file2
  stop 1
end if
if(i_min.eq.0) label2 = label2 + 1


! check the number of atoms
if(k.ne.n_atoms) then
  write(*,*) "# ERROR: Cannot compare sequences with different number of atoms!"
  stop 1
end if


! check if any label is missing, and if needed fix it with a relabeling
relabel_flag = 1
i1 = 0
i2 = 0
do while(relabel_flag.eq.1)

  ! compute the number of domains
  n_domains  = maxval(label1)
  n_domains2 = maxval(label2)

  ! check if any label is missing
  allocate(map(n_domains), map2(n_domains2))
  map = 0
  map2 = 0
  do i=1,n_atoms
    map( label1(i)) = map( label1(i)) + 1
    map2(label2(i)) = map2(label2(i)) + 1  
  end do
  if(minval(map).eq.0) then
    write(*,'(A,I3,A,A20)') ' # WARNING: label ', minloc(map) &
      + i1 ,' missing in file ', file1
    ! try to fix the missing label
    call relabeling(n_atoms,n_domains,map,label1)
    i1 = i1 + 1
    relabel_flag = 1
  else if(minval(map2).eq.0) then
    write(*,'(A,I3,A,A20)') ' # WARNING: label ', minloc(map2) &
      + i2,' missing in file ', file2
    ! try to fix the missing label
    call relabeling(n_atoms,n_domains,map2,label2)
    i2 = i2 + 1
    relabel_flag = 1
  else
    relabel_flag = 0
  end if

  deallocate(map, map2)
end do



write(*,'(A,I4)') ' Comparing seq. 1, num. of labels = ', n_domains
write(*,'(A,I4)') ' with  sequence 2, num. of labels = ', n_domains2


! n_domains of seq.1 MUST BE .ge. than n_domains of seq.1
! if not, swap labels and everything else
if(n_domains2.lt.n_domains) then
  ! swap the number-of-domains counters
  k = n_domains2
  n_domains2 = n_domains
  n_domains = k
  ! swap the labels
  label2m = label2
  label2 = label1
  label1 = label2m
  ! keep track of swapping
  swap_flag = 1
end if


! compute the factorial of n_domains2
k = 1
do j=2,n_domains2 
  k = k*j
end do


! ! if it's going to take too much time, abort
! if(k.gt.N_STEP_MAX) then
!   write(*,*) '# ERROR: it is going to take too much time. '
!   write(*,*) '# Set a larger value for N_STEP_MAX. Steps needed = ', k
!   stop 1
! end if


write(*,*) 'Number of elements =   ', n_atoms
write(*,*) 'Number of iterations = ', k
write(*,*)


! build domain-intersection matrix M
allocate( M(n_domains, n_domains2)  )
M = 0
do n=1,n_atoms
  M(label1(n), label2(n)) = M(label1(n), label2(n)) + 1
end do


allocate(map(n_domains), map2(n_domains2))


! MAIN CYCLE

if(n_domains2.lt.N_DOMAINS_ROUGH_APPROACH) then ! 1) scan all possible labels permutations

  ! initialization
  do i=1,n_domains2
    map2(i) = i
  end do

  ratio = 0.
  max_ratio = -1.
  do i=1,N_STEP_MAX
    label2m = map2(label2) 
    call fast_seq_intersection(n_atoms, n_domains, n_domains2, map2, M, ratio)
  !   call sequences_intersection(n_atoms,label1,label2m,ratio_old)
  !   if(ratio_old.ne.ratio) stop '# ERROR in routine fast_seq_intersection!'
    if(ratio.gt.max_ratio) then
      max_ratio = ratio
      label2_aligned = label2m
    end if
  !   write(*,*) map2
    if(.not.nextp(n_domains2,map2)) goto 88
  end do

  write(*,*) "# ERROR: Taking too many steps. Set a larger value for N_STEP_MAX."
  stop 1

  88 continue

  ! compute the factorial of n_domains2
  k = 1
  do j=2,n_domains2 
    k = k*j
  end do
  if(i.ne.k) then
    write(*,*) "# ERROR: Check generator of permutations!"
    stop 1
  end if

else ! 2) perform only a limited search

  write(*,*) '# Number of labels is too large.'
  write(*,*) '# Performing rough search of best labeling.'
  write(*,*)

  ! for each label2, find the label1 with largest intersection value
  allocate( nn(n_domains2), nn_value(n_domains2) )
  call find_nearest_neighbors(n_domains, n_domains2, M, nn, nn_value)

  allocate( nn_value_old(n_domains2) )
  nn_value_old = nn_value

  ! sort (in ascending order) label2 with respect to the larger intersection value
  allocate( order(n_domains2) )
  call quick_sort(n_domains2, nn_value, order)

  ! quick check of ordering:
  do j=2,n_domains2
    if(nn_value_old(order(j)).lt.nn_value_old(order(j-1))) then
      write(*,*) "# ERROR: 666!"
      stop 1
    end if
  end do

  ! set the sub-optimal reference labeling
  call sub_optimal_labeling(n_domains,n_domains2,M,nn,order,map2)
  label2_aligned = map2(label2)

  ! compute the total intersection
  call fast_seq_intersection(n_atoms,n_domains,n_domains2,map2,M,max_ratio)

  ! do a cycle of pairwise label-swapping
  do i=1,n_domains2
    do j=i+1,n_domains2
      call label_swap(n_domains2,i,j,map2)
      call fast_seq_intersection(n_atoms,n_domains,n_domains2,map2,M,ratio)
      if(ratio.gt.max_ratio) then
	max_ratio = ratio
	label2_aligned = map2(label2)
        write(*,*) 'Pairwise label swapping found a better labeling!'
      end if
      call label_swap(n_domains2,i,j,map2)
    end do
  end do

  deallocate(nn, nn_value, nn_value_old, order)
end if


! OUTPUT

write(*,'(A,F5.3)') ' Similarity = ', max_ratio

OPEN(UNIT=111, FILE='similarity.out', STATUS='REPLACE', ACTION='WRITE')
write(111,*) max_ratio

OPEN(UNIT=112, FILE='label1_aligned.out', STATUS='REPLACE', ACTION='WRITE')
OPEN(UNIT=113, FILE='label2_aligned.out', STATUS='REPLACE', ACTION='WRITE')
if(swap_flag.eq.0) then
  do i=1,n_atoms
    write(112,'(I4)') label1(i)
    write(113,'(I4)') label2_aligned(i)
  end do
else 
  do i=1,n_atoms
    write(112,'(I4)') label2_aligned(i)
    write(113,'(I4)') label1(i)
  end do
end if


deallocate(map, map2, M)

end program

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine relabeling(n_atoms,n_domains,map,label)

  implicit none
  integer, intent(in) :: n_atoms, n_domains
  integer, intent(in) :: map(n_domains)
  integer, intent(inout) :: label(n_atoms)
  integer :: i,missing_label
  
  ! find the missing label
  missing_label = minloc(map,1)

  ! decrease the label > missing_label
  do i=1,n_atoms
    if(label(i).gt.missing_label) then
      label(i) = label(i) - 1
    end if
  end do

  return
end subroutine

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine sequences_intersection(n_atoms,label1,label2,ratio)

  implicit none
  integer, intent(in) :: n_atoms, label1(n_atoms), label2(n_atoms)
  integer i,yes
  real, intent(out) :: ratio

  yes = 0
  do i=1,n_atoms
    if(label1(i).eq.label2(i)) yes = yes + 1
  end do

  ratio = real(yes)/real(n_atoms)

  return
end subroutine

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine fast_seq_intersection(n_atoms,n_domains,n_domains2,map2,M,ratio)

  implicit none
  integer, intent(in) :: n_atoms, n_domains, n_domains2
  integer, intent(in) :: map2(n_domains2), M(n_domains, n_domains2)
  integer i,yes
  real, intent(out) :: ratio

  yes = 0
  do i=1,n_domains2
    if(map2(i).gt.n_domains) goto 55
    yes = yes + M( map2(i), i )
    55 continue
  end do

  ratio = real(yes)/real(n_atoms)

  return

end subroutine

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      function nextp(n,a)
      integer n,a,i,j,k,t
      logical nextp
      dimension a(n)
      i=n-1
   10 if(a(i).lt.a(i+1)) go to 20
      i=i-1
      if(i.eq.0) go to 20
      go to 10
   20 j=i+1
      k=n
   30 t=a(j)
      a(j)=a(k)
      a(k)=t
      j=j+1
      k=k-1
      if(j.lt.k) go to 30
      j=i
      if(j.ne.0) go to 40
      nextp=.false.
      return
   40 j=j+1
      if(a(j).lt.a(i)) go to 40
      t=a(i)
      a(i)=a(j)
      a(j)=t
      nextp=.true.
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine find_nearest_neighbors(n_domains, n_domains2, M, nn, nn_value)

  implicit none
  integer, intent(in) :: n_domains, n_domains2
  integer, intent(in) :: M(n_domains, n_domains2)
  integer, intent(out) :: nn(n_domains2)
  real,    intent(out) :: nn_value(n_domains2)
  integer :: i,j
  real :: temp

  nn = 0
  nn_value = 0.

  do j=1,n_domains2
    do i=1,n_domains
      temp = real( M(i,j) )
      if(temp.gt.nn_value(j)) then
	nn(j) = i
	nn_value(j) = temp
      end if
    end do
  end do

end subroutine find_nearest_neighbors

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine find_free_nn(N1,N2,assigned,M,lab,free_nn)

  integer, intent(in) :: N1, N2 
  integer, intent(in) :: assigned(N2), M(N1,N2)
  integer, intent(in) :: lab
  integer, intent(out) :: free_nn
  integer :: i, temp
  
  temp = -1
  do i=1,N1
    if(assigned(i).eq.0) then
      if(M(i,lab).gt.temp) then
        free_nn = i
        temp = M(i,lab)
      end if
    end if
  end do

  if(temp.eq.-1) then
    do i=N1+1,N2
      if(assigned(i).eq.0) then
        free_nn = i
        temp = 0
      end if
    end do
  end if

  if(temp.eq.-1) stop '# ERROR in subroutine find_free_nn()!'

end subroutine find_free_nn

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine sub_optimal_labeling(n_domains,n_domains2,M,nn,order,map2)
  ! it tries to map each label2 into the label1 with largest intersection
  ! if the latter is already assigned, it tries with the next label1
  ! with largest intersection, and so on

  implicit none
  integer, intent(in) :: n_domains, n_domains2
  integer, intent(in) :: M(n_domains, n_domains2)
  integer, intent(in) :: nn(n_domains2), order(n_domains2)
  integer, intent(out) :: map2(n_domains2)
  integer :: j,k,nn_k
  integer :: assigned(n_domains2)

  assigned = 0

  do j=n_domains2,1,-1
    k = order(j)
    nn_k = nn(k)
    ! check if the label has been already assigned
    if(assigned(nn_k).eq.0) then
      ! k is mapped into its nearest neighbor
      map2(k) = nn_k
      assigned(nn_k) = 1
    else
      ! k is mapped in the not-assigned-yet label with largest intersection value
      call find_free_nn(n_domains,n_domains2,assigned,M,k,nn_k)
      map2(k) = nn_k
      assigned(nn_k) = 1
    end if
  end do 


end subroutine sub_optimal_labeling


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

RECURSIVE SUBROUTINE quick_sort(N, list, order)

! Quick sort routine from:
! Brainerd, W.S., Goldberg, C.H. & Adams, J.C. (1990) "Programmer's Guide to
! Fortran 90", McGraw-Hill  ISBN 0-07-000248-7, pages 149-150.
! Modified by Alan Miller to include an associated integer array which gives
! the positions of the elements in the original order.

IMPLICIT NONE
integer, intent(in) :: N
REAL, DIMENSION (N), INTENT(IN OUT)  :: list
INTEGER, DIMENSION (N), INTENT(OUT)  :: order

! Local variable
INTEGER :: i

DO i = 1, SIZE(list)
  order(i) = i
END DO

CALL quick_sort_1(1, SIZE(list))

CONTAINS

RECURSIVE SUBROUTINE quick_sort_1(left_end, right_end)

INTEGER, INTENT(IN) :: left_end, right_end

!     Local variables
INTEGER             :: i, j, itemp
REAL                :: reference, temp
INTEGER, PARAMETER  :: max_simple_sort_size = 6

IF (right_end < left_end + max_simple_sort_size) THEN
  ! Use interchange sort for small lists
  CALL interchange_sort(left_end, right_end)

ELSE
  ! Use partition ("quick") sort
  reference = list((left_end + right_end)/2)
  i = left_end - 1; j = right_end + 1

  DO
    ! Scan list from left end until element >= reference is found
    DO
      i = i + 1
      IF (list(i) >= reference) EXIT
    END DO
    ! Scan list from right end until element <= reference is found
    DO
      j = j - 1
      IF (list(j) <= reference) EXIT
    END DO


    IF (i < j) THEN
      ! Swap two out-of-order elements
      temp = list(i); list(i) = list(j); list(j) = temp
      itemp = order(i); order(i) = order(j); order(j) = itemp
    ELSE IF (i == j) THEN
      i = i + 1
      EXIT
    ELSE
      EXIT
    END IF
  END DO

  IF (left_end < j) CALL quick_sort_1(left_end, j)
  IF (i < right_end) CALL quick_sort_1(i, right_end)
END IF

END SUBROUTINE quick_sort_1


SUBROUTINE interchange_sort(left_end, right_end)

INTEGER, INTENT(IN) :: left_end, right_end

!     Local variables
INTEGER             :: i, j, itemp
REAL                :: temp

DO i = left_end, right_end - 1
  DO j = i+1, right_end
    IF (list(i) > list(j)) THEN
      temp = list(i); list(i) = list(j); list(j) = temp
      itemp = order(i); order(i) = order(j); order(j) = itemp
    END IF
  END DO
END DO

END SUBROUTINE interchange_sort

END SUBROUTINE quick_sort

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

subroutine label_swap(N,i,j,map)

  implicit none
  integer, intent(in) :: N,i,j
  integer, intent(inout) :: map(N)
  integer :: map_i_old

  map_i_old = map(i)
  map(i) = map(j)
  map(j) = map_i_old

end subroutine label_swap
