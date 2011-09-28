;; returns cons cell whose car points to 'COOP or 'DEFECT and the 
;; cdr points to the history structure
;(defun decide-aanz (opID history last-op-decision opchange mychange)
;)

;; history is opponent specific and returns c or d
;; history of form ('c 'd 'd ...)
;(defun make-choice-aanz (func ophistory)

;)

;; returns a function (in its list form)
;(defun look-up-aanz (prob-data)
;)

;; returns a real number which is the value of the probability
;; distribution evaluated at x
;; assumes prob-data is sorted
(defun prob-aanz (prob-data mychoice opchoice x)
  (let ((d (cond ((and (eq mychoice 'c) (eq opchoice 'c)) (first prob-data))
                 ((and (eq mychoice 'c) (eq opchoice 'd)) (second prob-data))
                 ((and (eq mychoice 'd) (eq opchoice 'd)) (third prob-data))
                 (T nil))) 
        (bot (cond ((and (eq mychoice 'c) (eq opchoice 'c)) 80)
                 ((and (eq mychoice 'c) (eq opchoice 'd)) 0)
                 ((and (eq mychoice 'd) (eq opchoice 'd)) 40)
                 (T 100))) 
        (top (cond ((and (eq mychoice 'c) (eq opchoice 'c)) 100)
                 ((and (eq mychoice 'c) (eq opchoice 'd)) 100)
                 ((and (eq mychoice 'd) (eq opchoice 'd)) 90)
                 (T 100)))
        last)
    ; sort here if data unsorted
    (or (dolist (a d)
      (if (> a x) (return (/ (if last 1.0 .5) (* (length d) (- a (or last bot))))) (setf last a)))
        (if last (/ .5 (* (length d) (- top last))) (/ 1.0 (- top bot))))))


; returns random number between 0 and 1
(defun rand-aanz () 
  (/ (random 4294967296) 4294967296.0))
